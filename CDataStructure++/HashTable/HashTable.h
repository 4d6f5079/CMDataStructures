#pragma once

#include <iostream>
#include <string>
#include <array>
#include <memory>
#include <functional>
#include "../LinkedList/LinkedList.h"

template<typename K, typename V>
class HashTable
{
public:
	HashTable(const size_t capacity)
		: 
		capacity(capacity),
		hashTable(new LinkedList<V>*[capacity]())
	{
	}

	~HashTable()
	{
		for (size_t i = 0; i < capacity; ++i)
		{
			delete hashTable[i];
		}
		delete[] hashTable;
	}

	void put(const K& key, const V& value)
	{
		const auto idx = hashFunc(key);

		//std::cout << "Hash Function Index: " << idx << std::endl;
		//std::cout << "Putting (" << key << " , " << value << ") in the hash table" << std::endl;

		if (hashTable[idx] == nullptr)
		{
			hashTable[idx] = new LinkedList<V>(value);
		}
		else
		{
			static_cast<LinkedList<V>*>(hashTable[idx])->insertAtHead(value);
		}
	}
	
	LinkedList<V>& get(const K& key)
	{
		const auto idx = hashFunc(key);
		return *hashTable[idx];
	}

	void deleteKey(const K& key)
	{
		const auto idx = hashFunc(key);
		delete hashTable[idx];
		hashTable[idx] = nullptr;
	}

	size_t hashFunc(const K& key)
	{
		return std::hash<K>{}(key) % capacity;
	}

	void printBinsInfo() const
	{
		for (size_t i = 0; i < capacity; ++i)
		{
			const auto ll = static_cast<LinkedList<V>*>(hashTable[i]);
			if (ll != nullptr)
			{
				std::cout << "Bin: " << i << "\t" << "Values: " << ll->getSize() << std::endl;
			}
			else
			{
				std::cout << "[UNUSED] Bin: " << i << "\t" << "Values: 0" << std::endl;
			}
		}
	}

private:
	size_t capacity;
	LinkedList<V>** hashTable;
};
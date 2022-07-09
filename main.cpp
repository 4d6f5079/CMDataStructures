#include "CDataStructure++/HashTable.h"
#include "CDataStructure++/LinkedList.h"
#include <random>
#include <iostream>
#include <functional>

std::string randomStrGen(uint32_t length, int32_t rndNum)
{
	const static std::string charset = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ1234567890";
	std::string result;
	result.resize(length);

	for (size_t i = 0; i < length; ++i)
		result[i] = charset[rndNum % charset.length()];

	return result;
}

int main(int argc, char* argv[])
{	
	// This section is for testing the LinkedList
	/*{
		LinkedList<std::string> ll(std::string("Hallo"));
		ll.insertAtHead(std::string("Yo"));
		ll.insertAtHead(std::string("No"));
		ll.insertAtHead(std::string("No"));
		ll.insertAtHead(std::string("No"));
		ll.deleteNodesGivenData(std::string("No"));
		ll.deleteNodesGivenData(std::string("Hallo"));
		ll.printNodes();
		const auto getANode = ll.getNode(std::string("Yo"));
		if (getANode != nullptr)
		{
			std::cout << *(getANode) << std::endl;
		}
	}*/

	// Constants
	static constexpr auto LOOP_ITERATIONS_POPULATION = 10000000;
	static constexpr auto UNIFORM_DIST_RND_MIN = 0;
	static constexpr auto UNIFORM_DIST_RND_MAX = 500000;
	static constexpr uint32_t RND_STIRNGS_LEN = 25;
	static constexpr size_t HASH_TABLE_CAP = 15;

	// Initialize the uniform int generator and create binds to use the generator and random string generator
	// as functions
	std::default_random_engine generator;
	std::uniform_int_distribution<uint32_t> distribution(UNIFORM_DIST_RND_MIN, UNIFORM_DIST_RND_MAX);
	auto roll_dice = std::bind(distribution, generator);
	const auto getRandomString = std::bind(&randomStrGen, std::placeholders::_1, std::placeholders::_2);

	// Init hash table and a string to store 1 key for testing
	std::string k;
	HashTable<std::string, int32_t> ht(HASH_TABLE_CAP);

	// Popluating Ht in a loop with randomly generatored keys and values
	for (size_t i = 0; i < LOOP_ITERATIONS_POPULATION; ++i)
	{
		const auto newValue = roll_dice();
		const auto newKey = getRandomString(RND_STIRNGS_LEN, roll_dice());
		ht.put(newKey, newValue);

		// FOR TESTING
		if (i == 0)
		{
			k = newKey;
		}
	}

	// Some Informational print outs for debugging 
	ht.get(k).printNodes();
	ht.printBinsInfo();

	return 0;
}
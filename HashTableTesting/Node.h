#pragma once

#include <iostream>

template<typename T>
class Node
{
public:
	Node(const T& data)
		:
		data(data),
		next(nullptr)
	{
	}

	Node(const T& data, Node<T>* next)
		:
		data(data),
		next(next)
	{
	}

	~Node()
	{
		if (this->next != nullptr) {
			this->next = nullptr;
		}
	}

	friend std::ostream& operator<<(std::ostream& stream, const Node& node)
	{
		stream << "Node Addr: " << &node << ", Node Data: " << node.data << ", Node Next Addr: " << node.next;
		return stream;
	}

	T data;
	Node* next;
};
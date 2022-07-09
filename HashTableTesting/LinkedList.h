#pragma once

#include <iostream>
#include "Node.h"

template<typename V>
class LinkedList
{
public:
	LinkedList()
		:
		headNode(nullptr),
		//lastNode(headNode),
		size(0)
	{
	}

	LinkedList(const V& data)
		:
		headNode(new Node<V>(data)),
		//lastNode(headNode),
		size(1)
	{
		//std::cout << "Second constructor of the LinkedList is called" << " Mem addr : " << this << std::endl;
	}

	~LinkedList()
	{
		//std::cout << "LinkedList Destructor " << this << " called." << std::endl;
		const Node<V>* tempNext = headNode;
		while (tempNext != nullptr)
		{
			const auto temp = tempNext->next;
			delete tempNext;
			tempNext = temp;
		}
	}

	void printNodes(const uint32_t& depth = 5)
	{
		uint32_t currDepth = 0;
		Node<V>* currNode = this->headNode;

		while (currDepth < depth && currNode != nullptr)
		{
			std::cout << "\t---" << "\n";
			std::cout << " Node depth: " << currDepth << "\n";
			std::cout << " Node memory address: " << currNode << "\n";
			std::cout << " Node data: " << currNode->data << "\n";
			std::cout << "\t---" << "\n";

			if (currDepth + 1 >= depth || currNode->next == nullptr)
			{
				std::cout << std::endl;
			} else {
				std::cout << "\t|" << "\n";
				std::cout << "\t|" << "\n";
				std::cout << "\t|" << "\n";
				std::cout << "\t\\/" << std::endl;
			}

			currDepth++;
			currNode = currNode->next;
		}
	}

	//void insertNode(Node<V>* const node)
	//{
	//	if (this->headNode == nullptr)
	//	{
	//		this->headNode = node; // The head node mem. address points to the inserted node if head node is null.
	//	}
	//	else
	//	{
	//		this->lastNode->next = node; // modify the previous node to point to the next (now last) node.
	//		this->lastNode = node; // save ref to added node.
	//	}

	//	size++; // increment the size of the linked list.
	//}

	void insertAtHead(const V& data)
	{
		Node<V>* nextNode = new Node<V>(data); // create node out of the given data.

		if (this->headNode != nullptr)
		{
			nextNode->next = this->headNode; // assign next node to head node ref
		}

		this->headNode = nextNode;// save ref to added node.

		size++; // increment the size of the linked list.
	}

	void deleteAtHead()
	{
		auto tempNext = this->headNode->next;
		delete this->headNode;
		this->headNode = tempNext;
	}

	/*
	*	Assumption: type V already implemented the == operator for comparisons.
	*/
	Node<V>& getNode(const V& data)
	{
		if (this->headNode->data == data)
		{
			return *headNode;
		}
		else
		{
			Node<V>* nextNode = this->headNode;
			while (nextNode != nullptr)
			{
				if (nextNode->data == data)
				{
					return *nextNode;
				}
				nextNode = nextNode->next;
			}
		}
	}

	size_t getSize()
	{
		return this->size;
	}

private:
	Node<V>* headNode;
	size_t size;
};
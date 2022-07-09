#pragma once

#include <iostream>
#include <memory>
#include "Node.h"

template<typename V>
class LinkedList
{
public:
	LinkedList()
		:
		headNode(nullptr),
		size(0)
	{
	}

	LinkedList(const V& data)
		:
		headNode(new Node<V>(data)),
		size(1)
	{
	}

	~LinkedList()
	{
		Node<V>* tempNext = headNode;
		while (tempNext != nullptr)
		{
			auto temp = tempNext->next;
			delete tempNext;
			tempNext = temp;
		}
	}

	void printNodes(const size_t depth = 5)
	{
		size_t currDepth = 0;
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
	
	size_t deleteNodesGivenData(const V& data)
	{
		Node<V>* prevNode = nullptr;
		Node<V>* currNode = headNode;
		size_t amountNodesDeleted = NULL;

		while (currNode != nullptr)
		{
			if (currNode->data == data)
			{
				if (currNode == headNode)
				{
					// delete at head
					deleteAtHead();
					currNode = this->headNode;
				}
				else
				{
					// delete node somewhere in the linked list
					prevNode->next = currNode->next;
					delete currNode;
					currNode = prevNode->next;
				}
				amountNodesDeleted++;
			}
			else
			{
				// continue, there is nothing to delete
				prevNode = currNode;
				currNode = currNode->next;
			}
		}

		if (amountNodesDeleted)
		{
			return amountNodesDeleted;
		}
		else
		{
			std::cout << "Node with data: '" << data << "' does not exist." << std::endl;
			return NULL;
		}
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
	std::unique_ptr<Node<V>> getNode(const V& data)
	{
		Node<V>* nextNode = this->headNode;
		while (nextNode != nullptr)
		{
			if (nextNode->data == data)
			{
				return std::make_unique<Node<V>>(*nextNode);
			}
			nextNode = nextNode->next;
		}
		std::cout << "Node with data: '" << data << "' does not exist." << std::endl;
		return nullptr;
	}

	size_t getSize()
	{
		return this->size;
	}

private:
	Node<V>* headNode;
	size_t size;
};
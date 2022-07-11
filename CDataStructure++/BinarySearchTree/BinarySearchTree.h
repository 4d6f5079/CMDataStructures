#pragma once

#include <iostream>
#include "BinarySearchTreeNode.h"

template<typename T>
class BinarySearchTree
{
public:
	BinarySearchTree()
		:
		root(nullptr),
		depth(0)
	{
	}

	//BinaryTree(BinaryTreeNode<T>* root)
	//	:
	//	root(root),
	//	depth(1)
	//{	
	//}

	BinarySearchTree(const T& data)
		:
		root(new BinarySearchTreeNode<T>(data)),
		depth(1)
	{
	}

	~BinarySearchTree()
	{
		cleanUpTree(root);
		depth = -1;
	}

	void cleanUpTree(BinarySearchTreeNode<T>* currNode)
	{
		// Post-order traversal to delete and free up memory taken by each node.
		// First the left three and right tree are visited and deleted first and then the current node is deleted so 
		// that everything descendent node is deleted first before deleting the current node to not lose reference
		// and cause memory leaks.
		if (currNode != nullptr)
		{
			if (currNode->hasLeft())
			{
				cleanUpTree(currNode->getLeft());
			}

			if (currNode->hasRight())
			{
				cleanUpTree(currNode->getRight());
			}

			delete currNode;
		}
	}

	void removeNode(const T& data)
	{
		BinarySearchTreeNode<T>* currNode = root;
		while (currNode != nullptr)
		{
			if (currNode->data == data)
			{
				// TODO: need recursion helper functions that keeps track of parent node and the node to the left/right
			}
		}
	}

	void insertNode(const T& data)
	{
		const auto toAddNode = new BinarySearchTreeNode<T>(data);

		// if root is not pointing to a node yet, then just add the toAddNode as root.
		if (root == nullptr)
		{
			root = toAddNode;
		}
		else
		{
			BinarySearchTreeNode<T>* currNode = root;
			while (currNode != nullptr)
			{
				if (data < currNode->getData())
				{
					if (currNode->hasLeft())
					{
						currNode = currNode->getLeft();
					}
					else
					{
						currNode->setLeft(toAddNode);
						return;
					}						
				}
				else if (data > currNode->getData())
				{
					if (currNode->hasRight())
					{
						currNode = currNode->getRight();
					}
					else
					{
						currNode->setRight(toAddNode);
						return;
					}
				}
				else
				{
					// don't add a node with the same data value twice, just return
					return;
				}
			}
		}
	}

	BinarySearchTreeNode<T>* DfsRecursionHelper(const T& data, BinarySearchTreeNode<T>* currRoot)
	{
		if (currRoot != nullptr)
		{
			if (currRoot->getData() == data)
				return currRoot;

			if (currRoot->hasLeft())
			{
				auto foundLeft = DfsRecursionHelper(data, currRoot->getLeft());
				if (foundLeft)
					return foundLeft;
			}

			if (currRoot->getRight())
			{
				auto foundRight = DfsRecursionHelper(data, currRoot->getRight());
				if (foundRight)
					return foundRight;
			}
		}

		return nullptr;
	}

	BinarySearchTreeNode<T>* DFS(const T& data)
	{
		return DfsRecursionHelper(data, root);
	}

private:
	BinarySearchTreeNode<T>* root;
	size_t depth;
};
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
		cleanUp();
	}

	void cleanUp()
	{
		// TODO:
		delete root;
		depth = -1;
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

	//BinaryTreeNode<T>* findLeft(const T& data)
	//{
	//	// TODO:
	//}

	//BinaryTreeNode<T>* findRight(const T& data)
	//{
	//	// TODO:
	//}

	bool DFS(const T& data)
	{
		if (root != nullptr)
		{

		}
		else
		{

		}
	}

	bool doesNodeExist(const T& data)
	{
		if (root)
		{
			if (root->data == data)
				return true;
		}

	}

private:
	BinarySearchTreeNode<T>* root;
	size_t depth;
};
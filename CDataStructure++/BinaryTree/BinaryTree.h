#pragma once

#include <iostream>
#include "BinaryTreeNode.h"

template<typename T>
class BinaryTree
{
public:
	BinaryTree()
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

	BinaryTree(const T& data)
		:
		root(new BinaryTreeNode<T>(data)),
		depth(1)
	{
	}

	~BinaryTree()
	{
		cleanUp();
	}

	void cleanUp()
	{
		// TODO:
		delete root;
		depth = -1;
	}

private:
	BinaryTreeNode<T>* root;
	size_t depth;
};
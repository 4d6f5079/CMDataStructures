#pragma once
#include <AVLNode.h>
#include <BinarySearchTree.h>

template<typename T>
class AVLTree
{
public:
	AVLTree()
		:
		root(nullptr)
	{
	}

	AVLTree(const T& data)
		:
		root(new AVLNode<T>(data))
	{
	}

	// Delete constructors which may cause headache and bugs
	AVLTree(const AVLTree<T>&) = delete;
	AVLTree(AVLTree<T>&&) = delete;

	~AVLTree()
	{
		cleanUpTree(root);
	}

	void printTree(AVLNode<T>* node = nullptr)
	{
		std::cout << "Printing the AVL Tree\n";
		std::cout << "|-- = left node (value < parent value)\n";
		std::cout << "\\-- = right/root node (value > parent value)\n\n";
		if (node == nullptr)
		{
			if (root != nullptr)
				printTree("", root, false);
		}
		else
		{
			printTree("", node, false);
		}
	}

	void removeNode(
		const T& data, 
		AVLNode<T>* parentNode, 
		AVLNode<T>* currNode
	)
	{

	}

	/*
	*	
	*/
	void removeNode(const T& data)
	{
		return removeNode(data, nullptr, root);
	}
	
	void insertNode(
		const T& data,
		AVLNode<T>* currNode
	)
	{

	}

	AVLNode<T>* rotateLeft(AVLNode<T>* parentNode, AVLNode<T>* currNode)
	{
		// TODO:
	}

	/* 
	*	
	*/
	void insertNode(const T& data)
	{

	}

	AVLNode<T>* getRoot()
	{
		return this->root;
	}

private:
	// from https://stackoverflow.com/questions/36802354/print-binary-tree-in-a-pretty-way-using-c
	void printTree(const std::string& prefix, AVLNode<T>* node, bool isLeft)
	{
		if (node != nullptr)
		{
			std::cout << prefix;

			std::cout << (isLeft ? "|-- " : "\\-- ");

			// print the value of the node
			std::cout << "(" << node->getData() << ")" << std::endl;

			// enter the next tree level - left and right branch
			printTree(prefix + (isLeft ? "|   " : "    "), node->getLeft(), true);
			printTree(prefix + (isLeft ? "|   " : "    "), node->getRight(), false);
		}
	}

	inline std::tuple<AVLNode<T>*, AVLNode<T>*> findInorderSuccessorHelper(
		AVLNode<T>* parentNode,
		AVLNode<T>* currNode
	)
	{
		if (currNode != nullptr)
		{
			if (currNode->hasLeft())
			{
				return findInorderSuccessorHelper(currNode, currNode->getLeft());
			}
			else
			{
				return std::make_tuple(parentNode, currNode);
			}
		}
		return std::make_tuple(nullptr, nullptr);
	}

	inline void setChildFromParent(
		AVLNode<T>* parentNode,
		AVLNode<T>* childToSet,
		AVLNode<T>* newRefToSetTo
	)
	{
		if (parentNode != nullptr)
		{
			if (parentNode->getLeft() == childToSet)
			{
				parentNode->setLeft(newRefToSetTo);
			}
			else if (parentNode->getRight() == childToSet)
			{
				parentNode->setRight(newRefToSetTo);
			}
		}
	}

	void cleanUpTree(AVLNode<T>* currNode)
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

private:
	AVLNode<T>* root;
};
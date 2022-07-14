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

	// from https://stackoverflow.com/questions/36802354/print-binary-tree-in-a-pretty-way-using-c
	void printTree(const std::string& prefix, BinarySearchTreeNode<T>* node, bool isLeft)
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

	void printTree(BinarySearchTreeNode<T>* node = nullptr)
	{
		std::cout << "Printing the BST\n";
		std::cout << "|-- = left node (value < parent value)\n";
		std::cout << "\\-- = right/root node (value > parent value)\n\n";
		if (node == nullptr)
		{
			printTree("", root, false);
		}
		else
		{
			printTree("", node, false);
		}
	}

	BinarySearchTreeNode<T>* findInorderSuccessor(BinarySearchTreeNode<T>* parentNode, BinarySearchTreeNode<T>* currNode)
	{
		if (currNode != nullptr)
		{
			if (currNode->hasLeft())
			{
				return findInorderSuccessor(currNode, currNode->getLeft());
			}
			else
			{
				return currNode;
			}
		}
		return nullptr;
	}

	void removeNode(const T& data, BinarySearchTreeNode<T>* parentNode, BinarySearchTreeNode<T>* currNode)
	{
		if (currNode == nullptr)
		{
			return;
		}

		if (data < currNode->getData())
		{
			return removeNode(data, currNode, currNode->getLeft());
		} 
		else if (data > currNode->getData())
		{
			return removeNode(data, currNode, currNode->getRight());
		}
		else
		{
			// Current node contains the given data.
			// There are 4 options for deletion:
			//	1: currNode has no children -> let the parent point to nullptr and then delete the currNode
			//	2: currNode has only left child -> make left child the new currNode and delete the old currNode
			//	3: currNode has only right child -> make the parent left/right ref point to right child the currNode and delete the old currNode
			//	4: currNode has both children:
			//		4.1: if the direct right node of currNode does not have a left child, then:
			//			4.1.1: make the right node the new currNode
			//			4.1.2: make the child of new currNode the left child of old currNode
			//			4.1.3: make parent node point to new currNode
			//			4.1.4: delete old currNode
			//		4.1: if the direct right node of currNode does have a left child, then search for inorder successor node by traversing to the deepest left node
			//		4.2: make successor the new root of the tree
			//			4.2.1: if successor has right child, make the left child of parent of the successor point to that child
			if (!currNode->hasLeft() && !currNode->hasRight())
			{
				setChildFromParent(parentNode, currNode, nullptr);
				delete currNode;
			}
			else if (!currNode->hasRight() && currNode->hasLeft())
			{
				setChildFromParent(parentNode, currNode, currNode->getLeft());
				delete currNode;
			}
			else if (currNode->hasRight() && !currNode->hasLeft())
			{
				setChildFromParent(parentNode, currNode, currNode->getRight());
				delete currNode;
			}
			else
			{
				// TODO:
			}
		}
	}

	void removeNode(const T& data)
	{
		return removeNode(data, nullptr, root);
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

	inline void setChildFromParent(
		BinarySearchTreeNode<T>* parentNode, 
		BinarySearchTreeNode<T>* childToSet,
		BinarySearchTreeNode<T>* newRefToSetTo
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

	BinarySearchTreeNode<T>* DFS(const T& data, BinarySearchTreeNode<T>* currRoot)
	{
		if (currRoot != nullptr)
		{
			if (currRoot->getData() == data)
				return currRoot;

			if (currRoot->hasLeft())
			{
				auto foundLeft = DFS(data, currRoot->getLeft());
				if (foundLeft)
					return foundLeft;
			}

			if (currRoot->getRight())
			{
				auto foundRight = DFS(data, currRoot->getRight());
				if (foundRight)
					return foundRight;
			}
		}

		return nullptr;
	}

	BinarySearchTreeNode<T>* DFS(const T& data)
	{
		return DFS(data, root);
	}

private:
	

	BinarySearchTreeNode<T>* root;
	size_t depth;
};
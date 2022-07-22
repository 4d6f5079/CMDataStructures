#pragma once
#include <AVLNode.h>
#include <BinarySearchTree.h>
#include <cstddef>

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

	AVLNode<T>* removeNode(
		const T& data, 
		AVLNode<T>* parentNode, 
		AVLNode<T>* currNode
	)
	{
		// TODO:
	}

	/*
	*	
	*/
	void removeNode(const T& data)
	{
		const auto parentRemovedNodeRef = removeNode(data, nullptr, root);
		rebalanceTree(parentRemovedNodeRef);
	}

	/*
	* ROTATE LEFT CASE:
	*	<currNode> is a left child of its parent <parentNode> and BF(<currNode>) <= 0
	*/
	AVLNode<T>* rotateLeft(AVLNode<T>* parentNode, AVLNode<T>* currNode)
	{
		// currNode is by 2 higher than its sibling
		AVLNode<T>* innerChild = currNode->getLeft(); // Inner child of currNode
		parentNode->setRight(innerChild);

		if (innerChild != nullptr)
		{
			innerChild->setParent(parentNode);
		}

		currNode->setLeft(parentNode);
		parentNode->setParent(currNode);

		// 1st case, BF(current node) == 0,
		//   only happens with deletion, not insertion:
		if (currNode->getBf() == 0)
		{
			// t23 has been of same height as t4
			parentNode->setBf(1);   // t23 now higher
			currNode->setBf(-1);   // t4 now lower than parent node
		}
		else
		{ // 2nd case happens with insertion or deletion:
			parentNode->setBf(0);
			currNode->setBf(0);
		}

		return currNode; // return new root of rotated subtree
	}

	/*
	* ROTATE RIGHT CASE: 
	*	<currNode> is a right child of its parent <parentNode> and BF(<currNode>) >= 0
	*/
	AVLNode<T>* rotateRight(AVLNode<T>* parentNode, AVLNode<T>* currNode)
	{
		// currNode is by 2 higher than its sibling
		AVLNode<T>* innerChild = currNode->getRight(); // Inner child of currNode
		parentNode->setLeft(innerChild);

		if (innerChild != nullptr)
		{
			innerChild->setParent(parentNode);
		}

		currNode->setRight(parentNode);
		parentNode->setParent(currNode);

		// 1st case, BF(current node) == 0,
		//   only happens with deletion, not insertion:
		if (currNode->getBf() == 0)
		{
			// t23 has been of same height as t4
			parentNode->setBf(1);   // t23 now higher
			currNode->setBf(-1);   // t4 now lower than parent node
		}
		else
		{ // 2nd case happens with insertion or deletion:
			parentNode->setBf(0);
			currNode->setBf(0);
		}

		return currNode; // return new root of rotated subtree
	}

	AVLNode<T>* rotateLeftLeft()
	{
		// TODO: rotate left twice
		return nullptr;
	}

	AVLNode<T>* insertNode(
		const T& data, 
		AVLNode<T>* parentNode, 
		AVLNode<T>* currNode, 
		std::size_t height
	)
	{
		if (root == nullptr)
		{
			root = new AVLNode<T>(data);
			return root;
		} 
		else if (currNode != nullptr)
		{
			if (data < currNode->getData())
			{
				if (currNode->hasLeft())
				{
					return insertNode(data, currNode, currNode->getLeft(), height + 1);
				}
				else
				{
					currNode->setLeft(new AVLNode<T>(data, currNode, height + 1));
					return currNode->getLeft();
				}
			}
			else if (data > currNode->getData())
			{
				if (currNode->hasRight())
				{
					return insertNode(data, currNode, currNode->getRight(), height + 1);
				}
				else
				{
					currNode->setRight(new AVLNode<T>(data, currNode, height + 1));
					return currNode->getRight();
				}
			}
		}
		// don't add a node with the same data value twice, just return nullptr
		return nullptr;
	}

	void insertNode(const T& data)
	{
		const auto insertedNodeRef = insertNode(data, nullptr, root, 0);
		rebalanceTree(insertedNodeRef);
	}

	void rebalanceTree(AVLNode<T>* currNode)
	{
		// TODO: do the shifts of nodes here and modify the heights of each node along the path
	}

	//const signed char getBf(AVLNode<T>* node)
	//{
	//	if (node != nullptr)
	//	{
	//		return node->getRight()->getHeight() - node->getLeft()->getHeight();
	//	}
	//	return -11; // node is not present, return some number that is not in the incosistent/consistent cases  
	//}

	bool isRightChild(AVLNode<T>* parentNode, AVLNode<T>* nodeToCheck)
	{
		if (parentNode->getRight() == nodeToCheck)
		{
			return true;
		}
		return false;
	}

	AVLNode<T>* getRoot()
	{
		return this->root;
	}

	AVLNode<T>* DFS(const T& data)
	{
		return DFS(data, root);
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

	AVLNode<T>* DFS(const T& data, AVLNode<T>* currRoot)
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

	//inline void setChildFromParent(
	//	AVLNode<T>* parentNode,
	//	AVLNode<T>* childToSet,
	//	AVLNode<T>* newRefToSetTo
	//)
	//{
	//	if (parentNode != nullptr)
	//	{
	//		if (parentNode->getLeft() == childToSet)
	//		{
	//			parentNode->setLeft(newRefToSetTo);
	//		}
	//		else if (parentNode->getRight() == childToSet)
	//		{
	//			parentNode->setRight(newRefToSetTo);
	//		}
	//	}
	//}

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
#pragma once
#include <AVLNode.h>
#include <BinarySearchTree.h>
#include <cstddef>
#include <iostream>

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
		// TODO: implement deletion of node
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
	* SIMPLE ROTATION - LEFT CASE:
	*	Z (currNode) is a left child of its parent X (parentNode) and BF(Z) <= 0
	*/
	AVLNode<T>* rotateLeft(AVLNode<T>* parentNode, AVLNode<T>* currNode)
	{
		// currNode is by 2 higher than its sibling
		AVLNode<T>* innerChild = currNode->getLeft(); // Left child of currNode
		parentNode->setRight(innerChild);

		if (innerChild != nullptr)
		{
			innerChild->setParent(parentNode);
		}

		currNode->setLeft(parentNode);

		// Set new parent left/right of currNode to currNode itself so that ref from other nodes 
		// to currNode does not get lost
		auto parentParentNode = parentNode->getParent();
		setChildFromParent(parentParentNode, parentNode, currNode);

		currNode->setParent(parentParentNode);
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
	* SIMPLE ROTATION - RIGHT CASE: 
	*	Z (currNode) is a right child of its parent X (parentNode) and BF(Z) >= 0
	*/
	AVLNode<T>* rotateRight(AVLNode<T>* parentNode, AVLNode<T>* currNode)
	{
		// currNode is by 2 higher than its sibling
		AVLNode<T>* innerChild = currNode->getRight(); // Right child of currNode
		parentNode->setLeft(innerChild);

		if (innerChild != nullptr)
		{
			innerChild->setParent(parentNode);
		}

		currNode->setRight(parentNode);

		// Set new parent left/right of currNode to currNode itself so that ref from other nodes 
		// to currNode does not get lost
		auto parentParentNode = parentNode->getParent();
		setChildFromParent(parentParentNode, parentNode, currNode);

		currNode->setParent(parentParentNode);
		parentNode->setParent(currNode);

		// 1st case, BF(current node) == 0,
		//   only happens with deletion, not insertion:
		if (currNode->getBf() == 0)
		{
			// t23 has been of same height as t1
			parentNode->setBf(-1);   // t23 now higher
			currNode->setBf(1);   // t1 now lower than parent node
		}
		else
		{ // 2nd case happens with insertion or deletion:
			parentNode->setBf(0);
			currNode->setBf(0);
		}

		return currNode; // return new root of rotated subtree
	}

	/*
	* DOUBLE ROTATION - RIGHT_LEFT ROTATION:
	*	Z (currNode) is a right child of its parent X (parentNode) and BF(Z) < 0
	*/
	AVLNode<T>* rotateRightLeft(AVLNode<T>* parentNode, AVLNode<T>* currNode)
	{
		AVLNode<T>* innerChild = currNode->getLeft(); // Y
		AVLNode<T>* leftOfInnerChild = innerChild->getLeft(); // t2
		AVLNode<T>* rightOfInnerChild = innerChild->getRight(); // t3
		const auto innerChildBF = innerChild->getBf();

		if (innerChild != nullptr) // TODO: remove this if statement as it is assumed/expected that this node exists
		{
			// rotate right around currNode and then left around parentNode
			currNode->setLeft(rightOfInnerChild);
			if (rightOfInnerChild != nullptr)
			{
				rightOfInnerChild->setParent(currNode);
			}
			parentNode->setRight(leftOfInnerChild);
			if (leftOfInnerChild != nullptr)
			{
				leftOfInnerChild->setParent(parentNode);
			}
			innerChild->setLeft(parentNode);
			innerChild->setRight(currNode);

			// Set new parent left/right of innerChild to innerChild itself so that ref from other nodes 
			// to innerChild does not get lost
			auto parentParentNode = parentNode->getParent();
			setChildFromParent(parentParentNode, parentNode, innerChild);

			innerChild->setParent(parentParentNode);
			parentNode->setParent(innerChild);
			currNode->setParent(innerChild);

			// 1st case, BF(Y) == 0,
			//   only happens with deletion, not insertion:
			if (innerChildBF == 0)
			{
				parentNode->setBf(0);
				currNode->setBf(0);
			}
			else
			{
				// other cases happen with insertion or deletion:
				if (innerChildBF > 0)
				{
					// t3 was higher
					parentNode->setBf(-1); // t1 now higher
					currNode->setBf(0);
				}
				else
				{
					// t2 was higher
					parentNode->setBf(0);
					currNode->setBf(1); // t4 now higher
				}
			}

			innerChild->setBf(0);

			return innerChild;
		}

		std::cout << "[rotateRightLeft] innerChild is not valid (nullptr): " << innerChild << "\n";
		return nullptr;
	}

	/*
	* DOUBLE ROTATION - LEFT_RIGHT ROTATION:
	*	Z (currNode) is a left child of its parent X (parentNode) and BF(Z) > 0
	*/
	AVLNode<T>* rotateLeftRight(AVLNode<T>* parentNode, AVLNode<T>* currNode)
	{
		AVLNode<T>* innerChild = currNode->getRight(); // Y
		AVLNode<T>* leftOfInnerChild = innerChild->getLeft(); // t3
		AVLNode<T>* rightOfInnerChild = innerChild->getRight(); // t2
		const auto innerChildBF = innerChild->getBf();

		if (innerChild != nullptr) // TODO: remove this if statement as it is assumed/expected that this node exists
		{
			// rotate right around currNode and then left around parentNode
			currNode->setRight(leftOfInnerChild);
			if (leftOfInnerChild != nullptr)
			{
				leftOfInnerChild->setParent(currNode);
			}
			parentNode->setLeft(rightOfInnerChild);
			if (rightOfInnerChild != nullptr)
			{
				rightOfInnerChild->setParent(parentNode);
			}
			innerChild->setRight(parentNode);
			innerChild->setLeft(currNode);

			// Set new parent left/right of innerChild to innerChild itself so that ref from other nodes 
			// to innerChild does not get lost
			auto parentParentNode = parentNode->getParent();
			setChildFromParent(parentParentNode, parentNode, innerChild);

			innerChild->setParent(parentParentNode);
			parentNode->setParent(innerChild);
			currNode->setParent(innerChild);

			// 1st case, BF(Y) == 0,
			//   only happens with deletion, not insertion:
			if (innerChildBF == 0)
			{
				parentNode->setBf(0);
				currNode->setBf(0);
			}
			else
			{
				// other cases happen with insertion or deletion:
				if (innerChildBF > 0)
				{
					// t2 was higher
					parentNode->setBf(0); 
					currNode->setBf(-1); // t4 now higher
				}
				else
				{
					// t3 was higher
					parentNode->setBf(1); // t1 now higher
					currNode->setBf(0); 
				}
			}

			innerChild->setBf(0);

			return innerChild;
		}

		std::cout << "[rotateLeftRight] innerChild is not valid (nullptr): " << innerChild << "\n";
		return nullptr;
	}

	AVLNode<T>* insertNode(
		const T& data, 
		AVLNode<T>* parentNode, 
		AVLNode<T>* currNode 
		//std::size_t height
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
					//return insertNode(data, currNode, currNode->getLeft(), height + 1);
					return insertNode(data, currNode, currNode->getLeft());
				}
				else
				{
					//currNode->setLeft(new AVLNode<T>(data, currNode, height + 1));
					currNode->setLeft(new AVLNode<T>(data, currNode));
					return currNode->getLeft();
				}
			}
			else if (data > currNode->getData())
			{
				if (currNode->hasRight())
				{
					//return insertNode(data, currNode, currNode->getRight(), height + 1);
					return insertNode(data, currNode, currNode->getRight());
				}
				else
				{
					//currNode->setRight(new AVLNode<T>(data, currNode, height + 1));
					currNode->setRight(new AVLNode<T>(data, currNode));
					return currNode->getRight();
				}
			}
		}

		// don't add a node with the same data value twice, just return nullptr
		return nullptr;
	}

	void insertNode(const T& data)
	{
		//const auto insertedNodeRef = insertNode(data, nullptr, root, 0);
		const auto insertedNodeRef = insertNode(data, nullptr, root);
		
		if (insertedNodeRef)
			rebalanceTree(insertedNodeRef);
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
			std::cout << "(" << node->getData() << ", bf: " << (int)node->getBf() << ")" << std::endl;

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

	inline void rebalanceTree(AVLNode<T>* parentNode, AVLNode<T>* currNode, const signed char bfDiff)
	{
		// increment/decrement bf value of parent node
		parentNode->setBf(parentNode->getBf() + bfDiff);

		const auto bfParent = parentNode->getBf();

		// the parent has unbalanced subtrees (invariant is violated)
		if (bfParent < -1 || bfParent > 1)
		{
			//The rebalancing is performed differently :
			//	Right Right	- X is rebalanced with a simple	rotation rotate_Left (see figure 2)
			//	Left Left	- X is rebalanced with a simple	rotation rotate_Right (mirror - image of figure 2)
			//	Right Left	- X is rebalanced with a double	rotation rotate_RightLeft (see figure 3)
			//	Left Right	- X is rebalanced with a double	rotation rotate_LeftRight (mirror - image of figure 3)

			if (parentNode->getRight() == currNode && currNode->getBf() >= 0) // Right Right	- Z is a right	child of its parent X and BF(Z) >= 0 
			{
				if (parentNode == root)
				{
					root = rotateLeft(parentNode, currNode);
				}
				else
				{
					rotateLeft(parentNode, currNode);
				}
			}
			else if (parentNode->getRight() == currNode && currNode->getBf() < 0) // Right Left	- Z is a right	child of its parent X and BF(Z) < 0 
			{
				if (parentNode == root)
				{
					root = rotateRightLeft(parentNode, currNode);
				}
				else
				{
					rotateRightLeft(parentNode, currNode);
				}
			}
			else if (parentNode->getLeft() == currNode && currNode->getBf() <= 0)  // Left Left	- Z is a left	child of its parent X and BF(Z) <= 0
			{
				if (parentNode == root)
				{
					root = rotateRight(parentNode, currNode);
				}
				else
				{
					rotateRight(parentNode, currNode);
				}
			}
			else if (parentNode->getLeft() == currNode && currNode->getBf() > 0) // Left Right	- Z is a left	child of its parent X and BF(Z) > 0
			{
				if (parentNode == root)
				{
					root = rotateLeftRight(parentNode, currNode);
				}
				else
				{
					rotateLeftRight(parentNode, currNode);
				}
			}
			else
			{
				std::cout << "[rebalanceTree] bfParent: " << bfParent << " , the else branch is reached which should not happen!";
			}
		}
		else // tree from parent node is balanced (invariant holds true), no need for rotation
		{
			auto parentParentNode = parentNode->getParent();
			
			// no updates possible to a parentNode if we are at the root
			if (parentParentNode == nullptr)
			{
				return;
			}

			// recurse upwards with new parent node, change bfDiff to +1 if going from right-up the tree
			// or -1 if going from left-up direction. This is done so that bf values are correctly modified
			// as the increment or decrement of the bf value depends on whether we go up from the right of the left node.
			if (isRightChild(parentParentNode, parentNode))
			{
				return rebalanceTree(parentParentNode, parentNode, constexpr signed char(1));
			}
			else
			{
				return rebalanceTree(parentParentNode, parentNode, constexpr signed char(-1));
			}
		}
	}

	void rebalanceTree(AVLNode<T>* insertedNode)
	{
		AVLNode<T>* parentCurrNode = insertedNode->getParent();
		if (parentCurrNode != nullptr)
		{
			if (isRightChild(parentCurrNode, insertedNode))
			{
				// if node is inserted to the right, then add 1 to direct parent node
				rebalanceTree(parentCurrNode, insertedNode, constexpr signed char(1));
			}
			else
			{
				// if node is inserted to the left, then subtract 1 to direct parent node
				rebalanceTree(parentCurrNode, insertedNode, constexpr signed char(-1));
			}
		}
	}

	inline bool isRightChild(AVLNode<T>* parentNode, AVLNode<T>* nodeToCheck)
	{
		if (parentNode->getRight() == nodeToCheck)
		{
			return true;
		}
		return false;
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
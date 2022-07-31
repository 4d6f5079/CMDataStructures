#pragma once
#include <AVLNode.h>
#include <cstddef>
#include <iostream>
#include <utility>

template <typename T>
class AVLTree
{
public:
	// typedef pair containing the deleted root and whether it was deleted from the right direction
	using _AVL_fromRight_Pair = std::pair<AVLNode<T> *, bool>;

public:
	AVLTree()
		: root(nullptr)
	{
	}

	AVLTree(const T &data)
		: root(new AVLNode<T>(data))
	{
	}

	// Delete constructors which may cause headache and bugs
	AVLTree(const AVLTree<T> &) = delete;
	AVLTree(AVLTree<T> &&) = delete;

	~AVLTree()
	{
		cleanUpTree(root);
	}

	void printTree(AVLNode<T> *node = nullptr)
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

	/*
	 *	Remove node with given data. Rebalance tree appropriately.
	 */
	void removeNode(const T &data)
	{
		_AVL_fromRight_Pair retValue = removeNode(data, root);
		auto parentRemovedNodeRef = retValue.first;
		auto isDeletedFromRightTree = retValue.second;

		if (parentRemovedNodeRef)
			rebalanceTreeDeletion(parentRemovedNodeRef, isDeletedFromRightTree);
	}

	void insertNode(const T &data)
	{
		const auto insertedNodeRef = insertNode(data, root);

		if (insertedNodeRef)
			rebalanceTreeInsertion(insertedNodeRef);
	}

	AVLNode<T> *getRoot()
	{
		return this->root;
	}

	AVLNode<T> *searchNode(const T &data)
	{
		return searchNode(data, root);
	}

	inline AVLNode<T> *findInorderSuccessor(AVLNode<T> *rightNodeOfCurrNode)
	{
		if (rightNodeOfCurrNode != nullptr)
		{
			if (rightNodeOfCurrNode->hasLeft())
			{
				return findInorderSuccessor(rightNodeOfCurrNode->getLeft());
			}
			else
			{
				return rightNodeOfCurrNode;
			}
		}

		return nullptr;
	}

private:
	// from https://stackoverflow.com/questions/36802354/print-binary-tree-in-a-pretty-way-using-c
	void printTree(const std::string &prefix, AVLNode<T> *node, bool isLeft)
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

	AVLNode<T> *searchNode(const T &data, AVLNode<T> *currRoot)
	{
		if (currRoot != nullptr)
		{
			const auto currRootData = currRoot->getData();

			if (currRootData == data)
				return currRoot;

			// search to the right if data > current data node
			if (data > currRootData)
			{
				return searchNode(data, currRoot->getRight());
			}

			// search to the left if data < current data node
			if (data < currRootData)
			{
				return searchNode(data, currRoot->getLeft());
			}
		}

		return nullptr;
	}

	inline bool isRightChild(AVLNode<T> *parentNode, AVLNode<T> *nodeToCheck)
	{
		if (parentNode->getRight() == nodeToCheck)
		{
			return true;
		}
		return false;
	}

	inline void setChildFromParent(
		AVLNode<T> *parentNode,
		AVLNode<T> *childToSet,
		AVLNode<T> *newRefToSetTo)
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

	void rebalanceTreeDeletion(AVLNode<T> *parentDeletedNode, bool rightIsDeleted)
	{
		if (rightIsDeleted)
		{
			// if node is deleted from the right subtree, then subtract 1 to current parent node
			rebalanceTreeDeletion(parentDeletedNode, DECREMENT_BF);
		}
		else
		{
			// if node is deleted from the left subtree, then increment 1 to current parent node
			rebalanceTreeDeletion(parentDeletedNode, INCREMENT_BF);
		}
	}

	void rebalanceTreeInsertion(AVLNode<T> *insertedNode)
	{
		AVLNode<T> *parentCurrNode = insertedNode->getParent();

		// if the root is inserted, then no updates are needed since the tree is already balanced.
		if (parentCurrNode != nullptr)
		{
			if (isRightChild(parentCurrNode, insertedNode))
			{
				// if node is inserted to the right, then add 1 to direct parent node
				rebalanceTreeInsertion(parentCurrNode, insertedNode, INCREMENT_BF);
			}
			else
			{
				// if node is inserted to the left, then subtract 1 to direct parent node
				rebalanceTreeInsertion(parentCurrNode, insertedNode, DECREMENT_BF);
			}
		}
	}

	/*
	 * SIMPLE ROTATION - LEFT CASE:
	 *	Z (currNode) is a left child of its parent X (parentNode) and BF(Z) <= 0
	 */
	AVLNode<T> *rotateLeft(AVLNode<T> *parentNode, AVLNode<T> *currNode)
	{
		// currNode is by 2 higher than its sibling
		AVLNode<T> *innerChild = currNode->getLeft(); // Left child of currNode
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
			parentNode->setBf(1); // t23 now higher
			currNode->setBf(-1);  // t4 now lower than parent node
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
	AVLNode<T> *rotateRight(AVLNode<T> *parentNode, AVLNode<T> *currNode)
	{
		// currNode is by 2 higher than its sibling
		AVLNode<T> *innerChild = currNode->getRight(); // Right child of currNode
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
			parentNode->setBf(-1); // t23 now higher
			currNode->setBf(1);	   // t1 now lower than parent node
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
	AVLNode<T> *rotateRightLeft(AVLNode<T> *parentNode, AVLNode<T> *currNode)
	{
		AVLNode<T> *innerChild = currNode->getLeft();			// Y
		AVLNode<T> *leftOfInnerChild = innerChild->getLeft();	// t2
		AVLNode<T> *rightOfInnerChild = innerChild->getRight(); // t3
		const auto innerChildBF = innerChild->getBf();

		// if (innerChild != nullptr) // FOR DEBUGGING: it is assumed/expected that this node exists
		// {
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
		// }
		// std::cout << "[rotateRightLeft] innerChild is not valid (nullptr): " << innerChild << "\n";
		// return nullptr;
	}

	/*
	 * DOUBLE ROTATION - LEFT_RIGHT ROTATION:
	 *	Z (currNode) is a left child of its parent X (parentNode) and BF(Z) > 0
	 */
	AVLNode<T> *rotateLeftRight(AVLNode<T> *parentNode, AVLNode<T> *currNode)
	{
		AVLNode<T> *innerChild = currNode->getRight();			// Y
		AVLNode<T> *leftOfInnerChild = innerChild->getLeft();	// t3
		AVLNode<T> *rightOfInnerChild = innerChild->getRight(); // t2
		const auto innerChildBF = innerChild->getBf();

		// if (innerChild != nullptr) // FOR DEBUGGING: it is assumed/expected that this node exists
		// {
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
		// }
		// std::cout << "[rotateLeftRight] innerChild is not valid (nullptr): " << innerChild << "\n";
		// return nullptr;
	}

	inline void rebalanceTreeInsertion(
		AVLNode<T> *parentNode,
		AVLNode<T> *currNode,
		const signed char bfDiff)
	{
		// increment/decrement bf value of parent node
		parentNode->setBf(parentNode->getBf() + bfDiff);

		const auto bfParent = parentNode->getBf();

		// Insertion: stop if during insertion and after modifying bf value of parent
		// the bf value becomes 0
		if (bfParent == 0)
		{
			return;
		}

		// the parent has unbalanced subtrees (invariant is violated)
		if (bfParent < -1 || bfParent > 1)
		{
			// FROM WIKIPEDIA:
			// The rebalancing is performed differently :
			//	Right Right	- X is rebalanced with a simple	rotation rotate_Left
			//	Left Left	- X is rebalanced with a simple	rotation rotate_Right
			//	Right Left	- X is rebalanced with a double	rotation rotate_RightLeft
			//	Left Right	- X is rebalanced with a double	rotation rotate_LeftRight

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
			else if (parentNode->getLeft() == currNode && currNode->getBf() <= 0) // Left Left	- Z is a left	child of its parent X and BF(Z) <= 0
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
				std::cout << "[rebalanceTreeInsertion] bfParent: " << bfParent << " , the else branch is reached which should not happen!";
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
				return rebalanceTreeInsertion(parentParentNode, parentNode, INCREMENT_BF);
			}
			else
			{
				return rebalanceTreeInsertion(parentParentNode, parentNode, DECREMENT_BF);
			}
		}
	}

	void rebalanceTreeDeletion(AVLNode<T> *currNode, const signed char bfDiff)
	{
		if (currNode == nullptr)
		{
			return;
		}

		// increment/decrement bf value of parent node
		currNode->setBf(currNode->getBf() + bfDiff);

		const auto currNodeBf = currNode->getBf();

		// This variable is only used when a rotation happened because of unbalanced tree
		// In this case next parent would be parent of returned node after rotation.
		AVLNode<T> *nextParentAfterRotation = nullptr;

		// Deletion: stop if after deletion of node and modifying bf value of parent of the deleted
		// node the bf value becomes -1 or +1
		if (currNodeBf == -1 || currNodeBf == 1)
		{
			return;
		}
		// the parent has unbalanced subtrees and is left-heavy (invariant is violated)
		// taking left child as child node for rotation
		else if (currNodeBf < -1)
		{
			AVLNode<T> *currNodeLeft = currNode->getLeft();
			const auto currNodeLeftBf = currNodeLeft->getBf();

			if (currNodeLeftBf <= 0) // Left Left	- Z is a left	child of its parent X and BF(Z) <= 0
			{
				nextParentAfterRotation = rotateRight(currNode, currNodeLeft);
				if (currNode == root)
				{
					root = nextParentAfterRotation;
				}
			}
			else if (currNodeLeftBf > 0) // Left Right	- Z is a left	child of its parent X and BF(Z) > 0
			{
				nextParentAfterRotation = rotateLeftRight(currNode, currNodeLeft);
				if (currNode == root)
				{
					root = nextParentAfterRotation;
				}
			}
			else
			{
				std::cout << "[rebalanceTreeDeletion] {currNodeBf < -1} should not ever come here. Bug detected";
			}
		}
		// the parent has unbalanced subtrees and is right-heavy (invariant is violated)
		else if (currNodeBf > 1)
		{
			AVLNode<T> *currNodeRight = currNode->getRight();
			const auto currNodeRightBf = currNodeRight->getBf();

			if (currNodeRightBf >= 0) // Right Right	- Z is a right	child of its parent X and BF(Z) >= 0
			{
				nextParentAfterRotation = rotateLeft(currNode, currNodeRight);
				if (currNode == root)
				{
					root = nextParentAfterRotation;
				}
			}
			else if (currNodeRightBf < 0) // Right Left	- Z is a right	child of its parent X and BF(Z) < 0
			{
				nextParentAfterRotation = rotateRightLeft(currNode, currNodeRight);
				if (currNode == root)
				{
					root = nextParentAfterRotation;
				}
			}
			else
			{
				std::cout << "[rebalanceTreeDeletion] {currNodeBf > 1} should not ever come here. Bug detected";
			}
		}

		AVLNode<T> *nextParent = nullptr;

		if (nextParentAfterRotation == nullptr)
		{
			// no rotation has occured, continue with parent of current node
			nextParent = currNode->getParent();
		}
		else
		{
			// a rotation has occured, continue with parent of returned rotated node
			nextParent = nextParentAfterRotation->getParent();
			// assign currNode to node after rotation since currNode (where we are now) changes after rotation
			// so this is done to keep parent and currNode aligned with each other so that isRightChild check is
			// done correctly.
			currNode = nextParentAfterRotation;
		}

		if (nextParent != nullptr)
		{
			if (isRightChild(nextParent, currNode))
			{
				return rebalanceTreeDeletion(nextParent, DECREMENT_BF);
			}
			else
			{
				return rebalanceTreeDeletion(nextParent, INCREMENT_BF);
			}
		}
	}

	AVLNode<T> *insertNode(
		const T &data,
		AVLNode<T> *currNode)
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
					return insertNode(data, currNode->getLeft());
				}
				else
				{
					currNode->setLeft(new AVLNode<T>(data, currNode));
					return currNode->getLeft();
				}
			}
			else if (data > currNode->getData())
			{
				if (currNode->hasRight())
				{
					return insertNode(data, currNode->getRight());
				}
				else
				{
					currNode->setRight(new AVLNode<T>(data, currNode));
					return currNode->getRight();
				}
			}
		}

		// don't add a node with the same data value twice, just return nullptr
		return nullptr;
	}

	/*
	 * Should be the same way as with deletion in BST.
	 * Regarding balance factor and flow of updates:
	 * 	1: The BF of successor node should be replaced with the BF of the removed node
	 *	2: After deletion, the parent before deletion of the node that is used to replace the deleted
	 *       node should be used to recursivly update BF values of parent nodes until BF of -1 or 1 is found
	 */
	_AVL_fromRight_Pair removeNode(
		const T &data,
		AVLNode<T> *currNode)
	{
		if (currNode == nullptr)
		{
			return std::make_pair(nullptr, false);
		}

		if (data < currNode->getData())
		{
			return removeNode(data, currNode->getLeft());
		}
		else if (data > currNode->getData())
		{
			return removeNode(data, currNode->getRight());
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
			//		4.2: if the direct right node of currNode does have a left child, then search for inorder successor node by traversing to the deepest left node
			//		4.3: make successor the new root of the tree
			//			4.3.1: if successor has right child, make the left child of parent of the successor point to that child

			// TODO: this if statement is only done to change root pointer which is not possible
			// by changing the currNode pointer. See if somehow reference of pointer can be passed instead
			// to change the copy of the pointer outside of the function to point somewhere else.
			if (currNode->getParent() == nullptr) // root contains the value we need to remove
			{
				if (!currNode->hasLeft() && !currNode->hasRight())
				{
					delete root;
					root = nullptr;
					return std::make_pair(root, false);
				}
				else if (!currNode->hasRight() && currNode->hasLeft())
				{
					AVLNode<T> *leftOfCurrNode = currNode->getLeft(); // get left child of root
					leftOfCurrNode->setParent(nullptr);				  // set parent of left child to nullptr
					leftOfCurrNode->setBf(root->getBf());			  // set bf to value of root
					root = leftOfCurrNode;							  // assign root to left child
					delete currNode;								  // delete previous root
					currNode = nullptr;
					return std::make_pair(root, false); // return pair with root and fromRight bool
				}
				else if (currNode->hasRight() && !currNode->hasLeft())
				{
					AVLNode<T> *rightOfCurrNode = currNode->getRight(); // get right child of root
					rightOfCurrNode->setParent(nullptr);				// set parent of right child to nullptr
					rightOfCurrNode->setBf(root->getBf());				// set bf to value of root
					root = rightOfCurrNode;								// assign root to right child
					delete currNode;									// delete previous root
					currNode = nullptr;
					return std::make_pair(root, true); // return pair with root and fromRight bool
				}
				else
				{
					AVLNode<T> *inorderSuccessorNode = findInorderSuccessor(currNode->getRight());
					AVLNode<T> *parentInorderSuccessorNode = inorderSuccessorNode->getParent();

					// successor node is the direct right node from the root
					if (inorderSuccessorNode == currNode->getRight())
					{
						inorderSuccessorNode->setLeft(root->getLeft()); // set left of successor to left subtree of root
						inorderSuccessorNode->setParent(nullptr);		// parent is now nullptr since successor is new root
						inorderSuccessorNode->setBf(root->getBf());		// bf value should be same as root node
						root = inorderSuccessorNode;					// assign root to inorder successor
						delete currNode;								// delete previous root
						currNode = nullptr;
						return std::make_pair(root, true); // return pair with inorder successor and fromRight bool
					}
					else // successor node is somewhere in the tree
					{
						AVLNode<T> *rightOfSuccessorNode = inorderSuccessorNode->getRight();
						parentInorderSuccessorNode->setLeft(rightOfSuccessorNode); // set left of parent successor to right of successor
						// if successor has right node, then make parent of right node the parent of successor
						if (rightOfSuccessorNode != nullptr)
						{
							rightOfSuccessorNode->setParent(parentInorderSuccessorNode);
						}
						// Set the parent of the successor node parent to the successor itself as it replaces the old root
						// which, if the parent of successor node is not updated, will be deleted and parent will point to nullptr
						if (parentInorderSuccessorNode->getParent() == root)
						{
							parentInorderSuccessorNode->setParent(inorderSuccessorNode);
						}
						inorderSuccessorNode->setLeft(root->getLeft());	  // set left subtree of prev. root to new root
						inorderSuccessorNode->setRight(root->getRight()); // set right subtree of prev. root to new root
						inorderSuccessorNode->setParent(nullptr);		  // set parent of inorder successor to nullptr since it is now the root
						inorderSuccessorNode->setBf(root->getBf());		  // bf value should be same as root node
						root = inorderSuccessorNode;					  // set inorder successor as the new root
						delete currNode;								  // delete previous root
						currNode = nullptr;
						return std::make_pair(parentInorderSuccessorNode, false); // return parent of successor with fromRight false since it is always the left child
					}
				}
			}
			else
			{
				AVLNode<T> *parentNode = currNode->getParent();

				if (!currNode->hasLeft() && !currNode->hasRight())
				{
					const auto isRightNode = isRightChild(parentNode, currNode); // make sure correct fromRight value is given as it can be right/left depending on parent ref
					setChildFromParent(parentNode, currNode, nullptr);			 // set parent node approperaite child to nullptr as it is deleted
					delete currNode;											 // delete the node that needs to be deleted
					currNode = nullptr;
					return std::make_pair(parentNode, isRightNode); // return parent node for rebalancing, fromRight bool does not matter
				}
				else if (!currNode->hasRight() && currNode->hasLeft())
				{
					AVLNode<T> *leftCurrNode = currNode->getLeft();
					leftCurrNode->setParent(parentNode);					// change parent of left child to parent of currNode
					leftCurrNode->setBf(currNode->getBf());					// set bf of left child to bf of currNode
					setChildFromParent(parentNode, currNode, leftCurrNode); // update parent ref to point to the new child
					delete currNode;										// delete currNode
					currNode = nullptr;
					return std::make_pair(leftCurrNode, false); // return new child of the parent for rebalancing with fromRight false
				}
				else if (currNode->hasRight() && !currNode->hasLeft())
				{
					AVLNode<T> *rightCurrNode = currNode->getRight();
					rightCurrNode->setParent(parentNode);					 // change parent of right child to parent of currNode
					rightCurrNode->setBf(currNode->getBf());				 // set bf of right child to bf of currNode
					setChildFromParent(parentNode, currNode, rightCurrNode); // update parent ref to point to the new child
					delete currNode;										 // delete currNode
					currNode = nullptr;
					return std::make_pair(rightCurrNode, true); // return new child of the parent for rebalancing with fromRight is true
				}
				else
				{
					AVLNode<T> *inorderSuccessorNode = findInorderSuccessor(currNode->getRight());
					AVLNode<T> *parentInorderSuccessorNode = inorderSuccessorNode->getParent();

					// successor node is the direct right node from the root
					if (inorderSuccessorNode == currNode->getRight())
					{
						inorderSuccessorNode->setLeft(currNode->getLeft()); // set left of successor to left subtree of currNode
						inorderSuccessorNode->setParent(parentNode);		// parent is now parentNode
						inorderSuccessorNode->setBf(currNode->getBf());		// bf value should be same as currNode
						setChildFromParent(parentNode, currNode, inorderSuccessorNode);
						delete currNode; // delete currNode
						currNode = nullptr;
						return std::make_pair(inorderSuccessorNode, true); // return pair with inorder successor and fromRight bool
					}
					else // successor node is somewhere in the tree
					{
						AVLNode<T> *rightOfSuccessorNode = inorderSuccessorNode->getRight();
						parentInorderSuccessorNode->setLeft(rightOfSuccessorNode); // set left of parent successor to right of successor
						// if successor has right node, then make parent of right node the parent of successor
						if (rightOfSuccessorNode != nullptr)
						{
							rightOfSuccessorNode->setParent(parentInorderSuccessorNode);
						}
						// Set the parent of the successor node parent to the successor itself as it replaces the currNode
						// which, if the parent of successor node is not updated, will be deleted and parent will point to nullptr
						if (parentInorderSuccessorNode->getParent() == currNode)
						{
							parentInorderSuccessorNode->setParent(inorderSuccessorNode);
						}
						inorderSuccessorNode->setLeft(currNode->getLeft());				// set left subtree of prev. root to new root
						inorderSuccessorNode->setRight(currNode->getRight());			// set right subtree of prev. root to new root
						inorderSuccessorNode->setParent(parentNode);					// set parent of inorder successor to parentNode
						inorderSuccessorNode->setBf(currNode->getBf());					// bf value should be same as inorder successor
						setChildFromParent(parentNode, currNode, inorderSuccessorNode); // set inorder successor as the new root
						delete currNode;												// delete previous root
						currNode = nullptr;
						return std::make_pair(parentInorderSuccessorNode, false); // return parent of successor with fromRight false since it is always the left child
					}
				}
			}
		}
	}

	void cleanUpTree(AVLNode<T> *currNode)
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
	AVLNode<T> *root;
	const signed char INCREMENT_BF = 1;
	const signed char DECREMENT_BF = -1;
};
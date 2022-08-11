#pragma once
#include <AVLNode.h>
#include <cstddef>
#include <iostream>
#include <utility>

template <typename T>
class AVLTree
{
private:
	AVLNode<T> *root;
	const signed char INCREMENT_BF = 1;
	const signed char DECREMENT_BF = -1;

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

	// Delete implicit constructors which may cause headache and bugs
	AVLTree(const AVLTree<T> &) = delete;
	AVLTree(AVLTree<T> &&) = delete;

	~AVLTree()
	{
		cleanUpTree(root);
	}

	/**
	 * @brief Prints the AVLTree.
	 *
	 * @param node The root node from which to start printing nodes.
	 * (default) If node is nullptr, then root of the tree is used as start node.
	 */
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

	/**
	 * @brief Remove node with given data. Rebalance tree appropriately.
	 *
	 * @details Time-complexity is O(Log(N))
	 *
	 * 	1 - removeNodeHelper: O(Log(N)) for lookup and O(1) for removal
	 * 	2 - rebalanceTree: O(Log(N)) for retracing and O(1) for rebalancing using rotations
	 */
	void removeNode(const T &data)
	{
		const auto retValue = removeNodeHelper(data);
		const auto parentRemovedNodeRef = retValue.first;
		const auto isDeletedFromRightTree = retValue.second;

		if (parentRemovedNodeRef)
			rebalanceTree(parentRemovedNodeRef, isDeletedFromRightTree, false);
	}

	/**
	 * @brief Inserts new node in the tree adhering to BST rules of insertion.
	 *
	 * @details Time-complexity is O(Log(N))
	 * 1 - insertNode: O(Log(N)) for finding right place to insert new node
	 * 2 - rebalanceTree: O(Log(N)) for retracing and O(1) for rebalancing using rotations
	 *
	 * @param data The data that the new node should contain
	 */
	void insertNode(const T &data)
	{
		const auto retValue = insertNode(data, root);
		const auto insertedNodeParent = retValue.first;
		const auto insertedFromRight = retValue.second;

		if (insertedNodeParent)
			rebalanceTree(insertedNodeParent, insertedFromRight, true);
	}

	/**
	 * @brief Get the Root object
	 *
	 * @return AVLNode<T>*
	 */
	AVLNode<T> *getRoot()
	{
		return this->root;
	}

	/**
	 * @brief Method for searching for a node in a tree given the data, starting from the root.
	 * 	Worst case time-complexity: O(log(N)), N = #nodes in tree
	 *
	 * @param data The data that is searched
	 * @return AVLNode<T>*: nullptr if the node is not found, else non-const pointer of the node
	 */
	AVLNode<T> *searchNode(const T &data)
	{
		AVLNode<T> *currNode = root;
		while (currNode != nullptr)
		{
			const auto currData = currNode->getData();
			if (currData == data)
			{
				return currNode;
			}
			else if (data > currData)
			{
				currNode = currNode->getRight();
			}
			else // data < currData
			{
				currNode = currNode->getLeft();
			}
		}
		return nullptr;
	}

	/**
	 * @brief Find the inorder successor of a node given the right node of that node.
	 *
	 * @param rightNodeOfCurrNode The right node of the node for which to search the inorder successor node.
	 * @return AVLNode<T>*: Pointer to inorder successor node
	 */
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

	inline bool isRightChild(AVLNode<T> *parentNode, AVLNode<T> *nodeToCheck)
	{
		if (parentNode != nullptr)
		{
			if (parentNode->getRight() == nodeToCheck)
			{
				return true;
			}
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

	void rebalanceTree(AVLNode<T> *retraceNode, const bool comingFromRight, const bool isInsertion)
	{
		if (comingFromRight)
		{
			if (isInsertion)
			{
				// if node is inserted to the right, then add 1 to direct parent node
				return rebalanceTree(retraceNode, INCREMENT_BF, isInsertion);
			}
			else
			{
				// if node is deleted from the right subtree, then subtract 1 to current parent node
				return rebalanceTree(retraceNode, DECREMENT_BF, isInsertion);
			}
		}
		else
		{
			if (isInsertion)
			{
				// if node is inserted to the left, then subtract 1 to direct parent node
				return rebalanceTree(retraceNode, DECREMENT_BF, isInsertion);
			}
			else
			{
				// if node is deleted from the left subtree, then increment 1 to current parent node
				return rebalanceTree(retraceNode, INCREMENT_BF, isInsertion);
			}
		}
	}

	/**
	 * @brief Rebalancing tree using rotations if the tree gets unbalanced (not adhering to the invariant) after insertion/deletion.
	 *
	 * @param currNode The current node being processed. This is changed to new currNode when a rotation happens because in that case
	 * 	the currNode does not correspond to the node that we were at since the height of the nodes changes. So to retrace from the same height
	 * 	the currNode is changed to the new currNode after rotation.
	 *
	 * @param bfDiff The balance factor value that the currNode bf value will change to (either decrement of increment by 1).
	 * @param isInsertion Indication of whether an insertion happened (true) or a deletion (false)
	 */
	void rebalanceTree(AVLNode<T> *currNode, const signed char bfDiff, const bool isInsertion)
	{
		// we reached root, no updates/rebalancing are possible thus just return
		if (currNode == nullptr)
		{
			return;
		}

		// increment/decrement bf value of currNode
		currNode->setBf(currNode->getBf() + bfDiff);

		const auto prevCurrNode = currNode;
		auto currNodeBf = currNode->getBf();
		auto rotationOccured = false;

		// the parent has unbalanced subtrees and is left-heavy (invariant is violated)
		// taking left child as child node for rotation
		if (currNodeBf < -1)
		{
			const auto currNodeLeft = currNode->getLeft();
			const auto currNodeLeftBf = currNodeLeft->getBf();

			if (currNodeLeftBf <= 0) // Left Left	- Z is a left	child of its parent X and BF(Z) <= 0
			{
				currNode = rotateRight(currNode, currNodeLeft);
			}
			else // Left Right	- Z is a left	child of its parent X and BF(Z) > 0
			{
				currNode = rotateLeftRight(currNode, currNodeLeft);
			}

			rotationOccured = true;
		}
		// the parent has unbalanced subtrees and is right-heavy (invariant is violated)
		else if (currNodeBf > 1)
		{
			const auto currNodeRight = currNode->getRight();
			const auto currNodeRightBf = currNodeRight->getBf();

			if (currNodeRightBf >= 0) // Right Right	- Z is a right	child of its parent X and BF(Z) >= 0
			{
				currNode = rotateLeft(currNode, currNodeRight);
			}
			else // Right Left	- Z is a right	child of its parent X and BF(Z) < 0
			{
				currNode = rotateRightLeft(currNode, currNodeRight);
			}

			rotationOccured = true;
		}

		if (rotationOccured)
		{
			// update currNode bf to new bf after rotation
			currNodeBf = currNode->getBf();

			// update root to the new root node if a rotation has occured.
			if (prevCurrNode == root)
			{
				root = currNode;
			}
		}

		// Deletion: stop if after deletion of node and modifying bf value of parent of the deleted or after rotation
		// node the bf value becomes -1 or +1
		// Insertion: stop if during insertion and after modifying bf value of parent or after rotation
		// the bf value becomes 0
		if (
			(!isInsertion && (currNodeBf == -1 || currNodeBf == 1)) ||
			(isInsertion && currNode->getBf() == 0))
		{
			return;
		}

		const auto nextParent = currNode->getParent();

		if (nextParent != nullptr)
		{
			if (isRightChild(nextParent, currNode))
			{
				if (isInsertion)
				{
					return rebalanceTree(nextParent, INCREMENT_BF, isInsertion);
				}
				else
				{
					return rebalanceTree(nextParent, DECREMENT_BF, isInsertion);
				}
			}
			else
			{
				if (isInsertion)
				{
					return rebalanceTree(nextParent, DECREMENT_BF, isInsertion);
				}
				else
				{
					return rebalanceTree(nextParent, INCREMENT_BF, isInsertion);
				}
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

	_AVL_fromRight_Pair insertNode(
		const T &data,
		AVLNode<T> *currNode)
	{
		if (root == nullptr)
		{
			root = new AVLNode<T>(data);
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
					return std::make_pair(currNode, false);
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
					return std::make_pair(currNode, true);
				}
			}
		}

		// don't add a node with the same data value twice, just return nullptr
		return std::make_pair(nullptr, false);
	}

	/**
	 * @brief Should be the same way as with deletion in BST.
	 * Regarding balance factor and flow of updates:
	 * 	1: The BF of successor node should be replaced with the BF of the removed node
	 *	2: After deletion, the parent before deletion of the node that is used to replace the deleted
	 *       node should be used to recursivly update BF values of parent nodes until BF of -1 or 1 is found
	 *
	 * Time-complexity: O(log(N)) for lookup and removal (search for inorder successor from root)
	 *
	 * @param data The node containing this data should be removed
	 * @return Pair containing the node for rebalancing and
	 *  	   bool for whether the node that is deleted is deleted from the right/left.
	 * 		   If the node to delete is not present in the tree, a pair containing (nullptr,false) is returned.
	 */
	_AVL_fromRight_Pair removeNodeHelper(const T &data)
	{
		// Step 1: lookup node to be deleted.
		AVLNode<T> *currNode = searchNode(data);

		// node not found, so nothing to remove thus return
		if (currNode == nullptr)
		{
			return std::make_pair(nullptr, false);
		}

		// Step 2: remove the node that needs to be deleted, fix references and replace the deleted node with the
		// 	approperiate node
		AVLNode<T> *parentNode = currNode->getParent();			// parent of the root of this (sub-)tree
		AVLNode<T> *retNode = nullptr;							// the node from which to start rebalancing
		AVLNode<T> *newRootAssignment = nullptr;				// the node that replaces the removed root of the AVLTree
		const bool isRootToBeRemoved = (parentNode == nullptr); // bool indicating whether the root will be removed or not

		// make sure correct fromRight value is given as it can be right/left depending on parent ref
		// TODO: check whether this can be programmatically done with isRightChild method
		//		 by changing parentNode where necessary
		bool deletedFromRight = false;

		if (!currNode->hasLeft() && !currNode->hasRight())
		{
			const auto isRightNode = isRightChild(parentNode, currNode); // make sure correct fromRight value is given as it can be right/left depending on parent ref
			setChildFromParent(parentNode, currNode, nullptr);			 // set parent node approperiate child to nullptr as it is deleted

			deletedFromRight = isRightNode;
			newRootAssignment = retNode = parentNode;
		}
		else if (!currNode->hasRight() && currNode->hasLeft())
		{
			AVLNode<T> *leftCurrNode = currNode->getLeft();
			leftCurrNode->setParent(parentNode);					// change parent of left child to parent of currNode
			leftCurrNode->setBf(currNode->getBf());					// set bf of left child to bf of currNode
			setChildFromParent(parentNode, currNode, leftCurrNode); // update parent ref to point to the new child

			newRootAssignment = retNode = leftCurrNode;
		}
		else if (currNode->hasRight() && !currNode->hasLeft())
		{
			AVLNode<T> *rightCurrNode = currNode->getRight();
			rightCurrNode->setParent(parentNode);					 // change parent of right child to parent of currNode
			rightCurrNode->setBf(currNode->getBf());				 // set bf of right child to bf of currNode
			setChildFromParent(parentNode, currNode, rightCurrNode); // update parent ref to point to the new child

			deletedFromRight = true;
			newRootAssignment = retNode = rightCurrNode;
		}
		else
		{
			AVLNode<T> *inorderSuccessorNode = findInorderSuccessor(currNode->getRight());
			AVLNode<T> *parentInorderSuccessorNode = inorderSuccessorNode->getParent();

			// successor node is the direct right node from the root
			if (inorderSuccessorNode == currNode->getRight())
			{
				AVLNode<T> *leftCurrNode = currNode->getLeft();
				inorderSuccessorNode->setLeft(leftCurrNode);	// set left of successor to left subtree of currNode
				inorderSuccessorNode->setParent(parentNode);	// parent is now parentNode
				inorderSuccessorNode->setBf(currNode->getBf()); // bf value should be same as currNode
				setChildFromParent(parentNode, currNode, inorderSuccessorNode);

				// set parent of left node of currNode to the new parent (inorderSuccessor)
				if (leftCurrNode != nullptr)
				{
					leftCurrNode->setParent(inorderSuccessorNode);
				}

				deletedFromRight = true;
				newRootAssignment = retNode = inorderSuccessorNode;
			}
			else // successor node is somewhere in the tree
			{
				AVLNode<T> *rightOfSuccessorNode = inorderSuccessorNode->getRight();
				AVLNode<T> *leftCurrNode = currNode->getLeft();
				AVLNode<T> *rightCurrNode = currNode->getRight();

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

				inorderSuccessorNode->setLeft(leftCurrNode);					// set left subtree of prev. root to new root
				inorderSuccessorNode->setRight(rightCurrNode);					// set right subtree of prev. root to new root
				inorderSuccessorNode->setParent(parentNode);					// set parent of inorder successor to parentNode
				inorderSuccessorNode->setBf(currNode->getBf());					// bf value should be same as inorder successor
				setChildFromParent(parentNode, currNode, inorderSuccessorNode); // set inorder successor as the new root

				// set parent of left and right nodes of currNode to the new currNode (inorderSuccessor)
				// so that the references don't break
				if (leftCurrNode != nullptr)
				{
					leftCurrNode->setParent(inorderSuccessorNode);
				}
				if (rightCurrNode != nullptr)
				{
					rightCurrNode->setParent(inorderSuccessorNode);
				}

				newRootAssignment = inorderSuccessorNode;
				retNode = parentInorderSuccessorNode;
			}
		}

		if (isRootToBeRemoved)
			root = newRootAssignment;

		// delete the node that needs to be deleted
		delete currNode;
		currNode = nullptr;

		// return parent node for rebalancing and fromRight bool indicating
		// from which direction the node got deleted
		return std::make_pair(retNode, deletedFromRight);
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
};
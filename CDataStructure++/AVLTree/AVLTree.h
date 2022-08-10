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
		const auto retValue = removeNodeHelper(data);
		const auto parentRemovedNodeRef = retValue.first;
		const auto isDeletedFromRightTree = retValue.second;

		if (parentRemovedNodeRef)
			rebalanceTree(parentRemovedNodeRef, isDeletedFromRightTree, false);
	}

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

	// TODO: this should be the new removeNode function
	// remove the found node, worst case: O(log(N)) for finding inorder successor from root of the tree
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
		AVLNode<T> *parentNode = currNode->getParent(); // parent of the root of this (sub-)tree
		AVLNode<T> *retNode = nullptr;					// the replacement node of the root that will be deleted
		const bool isRootToBeRemoved = (parentNode == nullptr);

		// make sure correct fromRight value is given as it can be right/left depending on parent ref
		// TODO: check whether this can be programmatically done with isRightChild method
		bool deletedFromRight = false;

		if (!currNode->hasLeft() && !currNode->hasRight())
		{
			setChildFromParent(parentNode, currNode, nullptr); // set parent node approperiate child to nullptr as it is deleted

			retNode = parentNode;
		}
		else if (!currNode->hasRight() && currNode->hasLeft())
		{
			AVLNode<T> *leftCurrNode = currNode->getLeft();
			leftCurrNode->setParent(parentNode);					// change parent of left child to parent of currNode
			leftCurrNode->setBf(currNode->getBf());					// set bf of left child to bf of currNode
			setChildFromParent(parentNode, currNode, leftCurrNode); // update parent ref to point to the new child

			retNode = leftCurrNode;
		}
		else if (currNode->hasRight() && !currNode->hasLeft())
		{
			AVLNode<T> *rightCurrNode = currNode->getRight();
			rightCurrNode->setParent(parentNode);					 // change parent of right child to parent of currNode
			rightCurrNode->setBf(currNode->getBf());				 // set bf of right child to bf of currNode
			setChildFromParent(parentNode, currNode, rightCurrNode); // update parent ref to point to the new child

			retNode = rightCurrNode;
			deletedFromRight = true;
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

				retNode = inorderSuccessorNode;
				deletedFromRight = true;
				// parentNode = parentInorderSuccessorNode;
				// std::cout << "parentNode == parentInorderSuccessorNode: " << (parentNode == parentInorderSuccessorNode) << std::endl; // TODO: check if this is even needed since parentNode is the same? delete this as it is for DEBUGGING ONLY
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

				retNode = parentInorderSuccessorNode;
				// parentNode = parentInorderSuccessorNode->getParent();
			}
		}

		if (isRootToBeRemoved)
			root = retNode;

		// delete the node that needs to be deleted
		delete currNode;
		currNode = nullptr;

		// return parent node for rebalancing and fromRight bool indicating
		// from which direction the node got deleted
		return std::make_pair(retNode, deletedFromRight);
	}

	/*
	 * Should be the same way as with deletion in BST.
	 * Regarding balance factor and flow of updates:
	 * 	1: The BF of successor node should be replaced with the BF of the removed node
	 *	2: After deletion, the parent before deletion of the node that is used to replace the deleted
	 *       node should be used to recursivly update BF values of parent nodes until BF of -1 or 1 is found
	 */
	// _AVL_fromRight_Pair removeNode(
	// 	const T &data,
	// 	AVLNode<T> *currNode)
	// {
	// 	if (currNode == nullptr)
	// 	{
	// 		return std::make_pair(nullptr, false);
	// 	}

	// 	if (data < currNode->getData())
	// 	{
	// 		return removeNode(data, currNode->getLeft());
	// 	}
	// 	else if (data > currNode->getData())
	// 	{
	// 		return removeNode(data, currNode->getRight());
	// 	}
	// 	else
	// 	{
	// 		// Current node contains the given data.
	// 		// There are 4 options for deletion:
	// 		//	1: currNode has no children -> let the parent point to nullptr and then delete the currNode
	// 		//	2: currNode has only left child -> make left child the new currNode and delete the old currNode
	// 		//	3: currNode has only right child -> make the parent left/right ref point to right child the currNode and delete the old currNode
	// 		//	4: currNode has both children:
	// 		//		4.1: if the direct right node of currNode does not have a left child, then:
	// 		//			4.1.1: make the right node the new currNode
	// 		//			4.1.2: make the child of new currNode the left child of old currNode
	// 		//			4.1.3: make parent node point to new currNode
	// 		//			4.1.4: delete old currNode
	// 		//		4.2: if the direct right node of currNode does have a left child, then search for inorder successor node by traversing to the deepest left node
	// 		//		4.3: make successor the new root of the tree
	// 		//			4.3.1: if successor has right child, make the left child of parent of the successor point to that child

	// 		// TODO: this if statement is only done to change root pointer which is not possible
	// 		// by changing the currNode pointer. See if somehow reference of pointer can be passed instead
	// 		// to change the copy of the pointer outside of the function to point somewhere else.
	// 		if (currNode->getParent() == nullptr) // root contains the value we need to remove
	// 		{
	// 			if (!currNode->hasLeft() && !currNode->hasRight())
	// 			{
	// 				delete root;
	// 				root = nullptr;
	// 				return std::make_pair(root, false);
	// 			}
	// 			else if (!currNode->hasRight() && currNode->hasLeft())
	// 			{
	// 				AVLNode<T> *leftOfCurrNode = currNode->getLeft(); // get left child of root
	// 				leftOfCurrNode->setParent(nullptr);				  // set parent of left child to nullptr
	// 				leftOfCurrNode->setBf(root->getBf());			  // set bf to value of root
	// 				root = leftOfCurrNode;							  // assign root to left child
	// 				delete currNode;								  // delete previous root
	// 				currNode = nullptr;
	// 				return std::make_pair(root, false); // return pair with root and fromRight bool
	// 			}
	// 			else if (currNode->hasRight() && !currNode->hasLeft())
	// 			{
	// 				AVLNode<T> *rightOfCurrNode = currNode->getRight(); // get right child of root
	// 				rightOfCurrNode->setParent(nullptr);				// set parent of right child to nullptr
	// 				rightOfCurrNode->setBf(root->getBf());				// set bf to value of root
	// 				root = rightOfCurrNode;								// assign root to right child
	// 				delete currNode;									// delete previous root
	// 				currNode = nullptr;
	// 				return std::make_pair(root, true); // return pair with root and fromRight bool
	// 			}
	// 			else
	// 			{
	// 				AVLNode<T> *inorderSuccessorNode = findInorderSuccessor(currNode->getRight());
	// 				AVLNode<T> *parentInorderSuccessorNode = inorderSuccessorNode->getParent();

	// 				// successor node is the direct right node from the root
	// 				if (inorderSuccessorNode == currNode->getRight())
	// 				{
	// 					inorderSuccessorNode->setLeft(root->getLeft()); // set left of successor to left subtree of root
	// 					inorderSuccessorNode->setParent(nullptr);		// parent is now nullptr since successor is new root
	// 					inorderSuccessorNode->setBf(root->getBf());		// bf value should be same as root node
	// 					root = inorderSuccessorNode;					// assign root to inorder successor
	// 					delete currNode;								// delete previous root
	// 					currNode = nullptr;
	// 					return std::make_pair(root, true); // return pair with inorder successor and fromRight bool
	// 				}
	// 				else // successor node is somewhere in the tree
	// 				{
	// 					AVLNode<T> *rightOfSuccessorNode = inorderSuccessorNode->getRight();
	// 					parentInorderSuccessorNode->setLeft(rightOfSuccessorNode); // set left of parent successor to right of successor
	// 					// if successor has right node, then make parent of right node the parent of successor
	// 					if (rightOfSuccessorNode != nullptr)
	// 					{
	// 						rightOfSuccessorNode->setParent(parentInorderSuccessorNode);
	// 					}
	// 					// Set the parent of the successor node parent to the successor itself as it replaces the old root
	// 					// which, if the parent of successor node is not updated, will be deleted and parent will point to nullptr
	// 					if (parentInorderSuccessorNode->getParent() == root)
	// 					{
	// 						parentInorderSuccessorNode->setParent(inorderSuccessorNode);
	// 					}
	// 					inorderSuccessorNode->setLeft(root->getLeft());	  // set left subtree of prev. root to new root
	// 					inorderSuccessorNode->setRight(root->getRight()); // set right subtree of prev. root to new root
	// 					inorderSuccessorNode->setParent(nullptr);		  // set parent of inorder successor to nullptr since it is now the root
	// 					inorderSuccessorNode->setBf(root->getBf());		  // bf value should be same as root node
	// 					root = inorderSuccessorNode;					  // set inorder successor as the new root
	// 					delete currNode;								  // delete previous root
	// 					currNode = nullptr;
	// 					return std::make_pair(parentInorderSuccessorNode, false); // return parent of successor with fromRight false since it is always the left child
	// 				}
	// 			}
	// 		}
	// 		else
	// 		{
	// 			AVLNode<T> *parentNode = currNode->getParent();

	// 			if (!currNode->hasLeft() && !currNode->hasRight())
	// 			{
	// 				const auto isRightNode = isRightChild(parentNode, currNode); // make sure correct fromRight value is given as it can be right/left depending on parent ref
	// 				setChildFromParent(parentNode, currNode, nullptr);			 // set parent node approperaite child to nullptr as it is deleted
	// 				delete currNode;											 // delete the node that needs to be deleted
	// 				currNode = nullptr;
	// 				return std::make_pair(parentNode, isRightNode); // return parent node for rebalancing, fromRight bool does not matter
	// 			}
	// 			else if (!currNode->hasRight() && currNode->hasLeft())
	// 			{
	// 				AVLNode<T> *leftCurrNode = currNode->getLeft();
	// 				leftCurrNode->setParent(parentNode);					// change parent of left child to parent of currNode
	// 				leftCurrNode->setBf(currNode->getBf());					// set bf of left child to bf of currNode
	// 				setChildFromParent(parentNode, currNode, leftCurrNode); // update parent ref to point to the new child
	// 				delete currNode;										// delete currNode
	// 				currNode = nullptr;
	// 				return std::make_pair(leftCurrNode, false); // return new child of the parent for rebalancing with fromRight false
	// 			}
	// 			else if (currNode->hasRight() && !currNode->hasLeft())
	// 			{
	// 				AVLNode<T> *rightCurrNode = currNode->getRight();
	// 				rightCurrNode->setParent(parentNode);					 // change parent of right child to parent of currNode
	// 				rightCurrNode->setBf(currNode->getBf());				 // set bf of right child to bf of currNode
	// 				setChildFromParent(parentNode, currNode, rightCurrNode); // update parent ref to point to the new child
	// 				delete currNode;										 // delete currNode
	// 				currNode = nullptr;
	// 				return std::make_pair(rightCurrNode, true); // return new child of the parent for rebalancing with fromRight is true
	// 			}
	// 			else
	// 			{
	// 				AVLNode<T> *inorderSuccessorNode = findInorderSuccessor(currNode->getRight());
	// 				AVLNode<T> *parentInorderSuccessorNode = inorderSuccessorNode->getParent();

	// 				// successor node is the direct right node from the root
	// 				if (inorderSuccessorNode == currNode->getRight())
	// 				{
	// 					inorderSuccessorNode->setLeft(currNode->getLeft()); // set left of successor to left subtree of currNode
	// 					inorderSuccessorNode->setParent(parentNode);		// parent is now parentNode
	// 					inorderSuccessorNode->setBf(currNode->getBf());		// bf value should be same as currNode
	// 					setChildFromParent(parentNode, currNode, inorderSuccessorNode);
	// 					delete currNode; // delete currNode
	// 					currNode = nullptr;
	// 					return std::make_pair(inorderSuccessorNode, true); // return pair with inorder successor and fromRight bool
	// 				}
	// 				else // successor node is somewhere in the tree
	// 				{
	// 					AVLNode<T> *rightOfSuccessorNode = inorderSuccessorNode->getRight();
	// 					parentInorderSuccessorNode->setLeft(rightOfSuccessorNode); // set left of parent successor to right of successor
	// 					// if successor has right node, then make parent of right node the parent of successor
	// 					if (rightOfSuccessorNode != nullptr)
	// 					{
	// 						rightOfSuccessorNode->setParent(parentInorderSuccessorNode);
	// 					}
	// 					// Set the parent of the successor node parent to the successor itself as it replaces the currNode
	// 					// which, if the parent of successor node is not updated, will be deleted and parent will point to nullptr
	// 					if (parentInorderSuccessorNode->getParent() == currNode)
	// 					{
	// 						parentInorderSuccessorNode->setParent(inorderSuccessorNode);
	// 					}
	// 					inorderSuccessorNode->setLeft(currNode->getLeft());				// set left subtree of prev. root to new root
	// 					inorderSuccessorNode->setRight(currNode->getRight());			// set right subtree of prev. root to new root
	// 					inorderSuccessorNode->setParent(parentNode);					// set parent of inorder successor to parentNode
	// 					inorderSuccessorNode->setBf(currNode->getBf());					// bf value should be same as inorder successor
	// 					setChildFromParent(parentNode, currNode, inorderSuccessorNode); // set inorder successor as the new root
	// 					delete currNode;												// delete previous root
	// 					currNode = nullptr;
	// 					return std::make_pair(parentInorderSuccessorNode, false); // return parent of successor with fromRight false since it is always the left child
	// 				}
	// 			}
	// 		}
	// 	}
	// }

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
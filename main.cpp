#include <HashTable.h>
#include <LinkedList.h>
#include <Timer.h>
#include <BinarySearchTree.h>
#include <AVLTree.h>
#include <random>
#include <iostream>
#include <functional>
#include <algorithm>
#include <exception>
#include <vector>

const std::string randomStrGen(const size_t &length, const size_t &rndNum)
{
	const static std::string charset = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ1234567890";
	std::string result;
	result.resize(length);

	for (size_t i = 0; i < length; ++i)
		result[i] = charset[rndNum % charset.length()];

	return result;
}

// This section is for testing the LinkedList
int testingLinkedList()
{
	try
	{
		LinkedList<std::string> ll(std::string("Hallo"));
		ll.insertAtHead(std::string("Yo"));
		ll.insertAtHead(std::string("No"));
		ll.insertAtHead(std::string("No"));
		ll.insertAtHead(std::string("No"));
		ll.deleteNodesGivenData(std::string("No"));
		ll.deleteNodesGivenData(std::string("Hallo"));
		ll.printNodes();
		const auto getANode = ll.getNode(std::string("Yo"));
		if (getANode != nullptr)
		{
			std::cout << *(getANode) << std::endl;
		}
		return 0;
	}
	catch (const std::exception &)
	{
		return -1;
	}
}

int testingHashTableWithBenchmark()
{
	// Constants
	static constexpr auto LOOP_ITERATIONS_POPULATION = 500;
	static constexpr auto UNIFORM_DIST_RND_MIN = 0;
	static constexpr auto UNIFORM_DIST_RND_MAX = 500000;
	static constexpr auto RND_STIRNGS_LEN = 5;
	static constexpr auto HASH_TABLE_CAP = 15;

	try
	{
		// Initialize the uniform int generator and create binds to use the generator and random string generator
		// as functions. This is done to have a uniform distribution of inputs so that a good hash can uniformaly distribute inputs
		// throughout the bins.
		std::random_device rd;																				 // obtain a random number from hardware
		std::mt19937 generator(rd());																		 // seed the generator
		std::uniform_int_distribution<std::size_t> distribution(UNIFORM_DIST_RND_MIN, UNIFORM_DIST_RND_MAX); // define the range
		auto roll_dice = std::bind(distribution, generator);
		const auto getRandomString = std::bind(&randomStrGen, std::placeholders::_1, std::placeholders::_2);

		// Init hash table
		HashTable<std::string, size_t> ht(HASH_TABLE_CAP);

		// Init vectors for randomly generated keys and values
		std::vector<std::string> rndStrs;
		std::vector<size_t> rndVals;
		rndStrs.clear();
		rndStrs.reserve(LOOP_ITERATIONS_POPULATION);
		rndVals.clear();
		rndVals.reserve(LOOP_ITERATIONS_POPULATION);

		for (size_t i = 0; i < LOOP_ITERATIONS_POPULATION; ++i)
		{
			rndStrs.emplace_back(getRandomString(RND_STIRNGS_LEN, roll_dice()));
			rndVals.emplace_back(roll_dice());
		}

		// Popluating Ht with benchmarking for put
		{
			Timer timer;
			for (size_t i = 0; i < LOOP_ITERATIONS_POPULATION; ++i)
			{
				ht.put(rndStrs.at(i), rndVals.at(i));
			}
		}

		// Some Informational print outs for debugging
		ht.get(rndStrs[0]).printNodes();
		ht.printBinsInfo();

		return 0;
	}
	catch (const std::exception &)
	{
		return -1;
	}
}

int testingBinarySearchTree()
{
	try
	{
		BinarySearchTree<float> bst;
		bst.insertNode(22);
		bst.insertNode(13);
		bst.insertNode(46);
		bst.insertNode(37);
		bst.insertNode(32);
		bst.insertNode(82);
		bst.insertNode(94);
		bst.insertNode(95);
		bst.insertNode(86);
		bst.insertNode(79);
		bst.insertNode(59);
		bst.insertNode(76);
		bst.insertNode(47);
		bst.insertNode(48);
		bst.insertNode(54);
		// std::cout << bst.DFS(5)->getData() << std::endl;
		std::cout << "Original Tree: \n";
		bst.printTree();
		std::cout << "removing leaf node case 1: \n";
		bst.removeNode(54);
		bst.printTree();
		std::cout << "\n\nremoving root node case 2: \n";
		bst.removeNode(22);
		bst.printTree();
		// std::cout << "\n\nremoving root node case 3: \n";
		// bst.printTree();
		return 0;
	}
	catch (const std::exception &)
	{
		return -1;
	}
}

int testAVLTreeSearchCases()
{
	AVLTree<int> t(7);
	t.insertNode(3);
	t.insertNode(10);
	t.insertNode(99);
	t.insertNode(-1);

	const auto getThree = t.searchNode(3);
	if (getThree->getData() == 3 &&
		getThree->getBf() == -1 &&
		getThree->getParent() == t.getRoot() &&
		getThree->hasLeft() &&
		!getThree->hasRight())
	{
		std::cout << "[CORRECT] getThree is found and adheres to the expected values\n";
	}
	else
	{
		std::cout << "[INCORRECT] getThree does not adhere to the expected values\n";
	}

	const auto ShouldNoExists = t.searchNode(100);
	if (ShouldNoExists == nullptr)
	{
		std::cout << "[CORRECT] ShouldNoExists node with value 100 is not in the tree.\n";
	}
	else
	{
		std::cout << "[INCORRECT] ShouldNoExists node with value 100 is found which should not be the case.\n";
	}

	const auto getNineNine = t.searchNode(99);
	if (getNineNine->getData() == 99 &&
		getNineNine->getBf() == 0 &&
		!getNineNine->hasLeft() &&
		!getNineNine->hasRight())
	{
		std::cout << "[CORRECT] getNineNine is found and adheres to the expected values\n";
	}
	else
	{
		std::cout << "[INCORRECT] getNineNine does not adhere to the expected values\n";
	}

	return 0;
}

int testAVLTreeInsertionCases()
{
	AVLTree<int> t(9);
	// t.insertNode(50);
	// t.insertNode(60);
	// t.insertNode(70);
	// t.insertNode(40);
	// t.insertNode(30);
	// t.insertNode(80);
	// t.insertNode(75);
	// t.insertNode(20);
	// t.insertNode(25);
	// t.insertNode(15);

	// t.insertNode(10);
	// t.insertNode(11);
	// t.insertNode(12);
	// t.insertNode(8);
	// t.insertNode(7);
	// t.insertNode(6);
	// t.insertNode(5);
	// t.insertNode(4);
	// t.insertNode(3);
	// t.insertNode(2);
	// t.insertNode(1);
	// t.insertNode(1);
	// t.insertNode(19);
	// t.insertNode(20);
	// t.insertNode(21);
	// t.insertNode(22);
	// t.insertNode(23);
	// t.insertNode(24);
	// t.insertNode(25);
	// t.insertNode(26);

	return 0;
}

int testAVLTreeDeletionCases()
{

	/**
	 * @brief [Case 1] Root deletion:
	 * 	tree becomes empty
	 */
	{
		AVLTree<int> t;
		t.insertNode(50);
		t.removeNode(50);
		// t.printTree();
		if (t.getRoot() == nullptr)
		{
			std::cout << "[CASE 1] CORRECT";
		}
		else
		{
			std::cout << "[CASE 1] INCORRECT";
		}
		std::cout << "\n";
	}

	/**
	 * @brief [Case 2] :
	 * 1: 2 nodes in tree (inc. root)
	 * 2: delete root
	 * 3: only root 55 in tree with bf 0
	 */
	{
		AVLTree<int> t;
		t.insertNode(50);
		t.insertNode(55);
		t.removeNode(50);
		// t.printTree();
		const auto root = t.getRoot();
		if (root->getBf() == 0 && root->getData() == 55)
		{
			std::cout << "[CASE 2] CORRECT";
		}
		else
		{
			std::cout << "[CASE 2] INCORRECT";
		}
		std::cout << "\n";
	}

	/**
	 * @brief [Case 3]:
	 * 1: 2 nodes in tree (inc. root)
	 * 2: delete root
	 * 3: only root 40 in tree with bf 0
	 */
	{
		AVLTree<int> t;
		t.insertNode(50);
		t.insertNode(40);
		t.removeNode(50);
		// t.printTree();
		const auto root = t.getRoot();
		if (root->getBf() == 0 && root->getData() == 40)
		{
			std::cout << "[CASE 3] CORRECT";
		}
		else
		{
			std::cout << "[CASE 3] INCORRECT";
		}
		std::cout << "\n";
	}

	/**
	 * @brief [Case 4]:
	 * 1: deleting root with right node while root having bf = -1
	 * 2: bf becomes -2
	 * 3: should rotate to the right to balance
	 * 4: new root = 7
	 */
	{
		AVLTree<int> t;
		std::vector<int> insertionsInOrder = {9, 7, 12, 5};
		for (auto i = 0; i < insertionsInOrder.size(); ++i)
		{
			t.insertNode(insertionsInOrder.at(i));
		}
		t.removeNode(9);
		// t.printTree();
		const auto root = t.getRoot();
		const auto leftRoot = root->getLeft();
		const auto rightRoot = root->getRight();
		if (root->getBf() == 0 &&
			root->getData() == 7 &&
			leftRoot->getBf() == 0 &&
			leftRoot->getData() == 5 &&
			rightRoot->getBf() == 0 &&
			rightRoot->getData() == 12)
		{
			std::cout << "[CASE 4] CORRECT";
		}
		else
		{
			std::cout << "[CASE 4] INCORRECT";
		}
		std::cout << "\n";
	}

	/**
	 * @brief [Case 5]:
	 * 1: deleting root (bf=0) with 2 right node
	 * 2: new root = 12 with bf = -1
	 * 3: no rotation should occur
	 * 4: new root = 12
	 */
	{
		AVLTree<int> t;
		std::vector<int> insertionsInOrder = {9, 7, 12, 5, 13};
		for (auto i = 0; i < insertionsInOrder.size(); ++i)
		{
			t.insertNode(insertionsInOrder.at(i));
		}
		t.removeNode(9);
		// t.printTree();
		const auto root = t.getRoot();
		const auto leftRoot = root->getLeft();
		const auto rightRoot = root->getRight();
		if (root->getBf() == -1 &&
			root->getData() == 12 &&
			leftRoot->getBf() == -1 &&
			leftRoot->getData() == 7 &&
			rightRoot->getBf() == 0 &&
			rightRoot->getData() == 13)
		{
			std::cout << "[CASE 5] CORRECT";
		}
		else
		{
			std::cout << "[CASE 5] INCORRECT";
		}
		std::cout << "\n";
	}

	/**
	 * @brief [Case 6]:
	 * 1: deleting root (bf=1) that has successor
	 * 2: successor 10 should take place of root with (bf=0)
	 * 3: new root = 10
	 */
	{
		AVLTree<int> t;
		std::vector<int> insertionsInOrder = {9, 5, 12, 10};
		for (auto i = 0; i < insertionsInOrder.size(); ++i)
		{
			t.insertNode(insertionsInOrder.at(i));
		}
		t.removeNode(9);
		// t.printTree();
		const auto root = t.getRoot();
		const auto leftRoot = root->getLeft();
		const auto rightRoot = root->getRight();
		if (root->getBf() == 0 &&
			root->getData() == 10 &&
			leftRoot->getBf() == 0 &&
			leftRoot->getData() == 5 &&
			rightRoot->getBf() == 0 &&
			rightRoot->getData() == 12)
		{
			std::cout << "[CASE 6] CORRECT";
		}
		else
		{
			std::cout << "[CASE 6] INCORRECT";
		}
		std::cout << "\n";
	}

	/**
	 * @brief [Case 7]:
	 * 1: deleting root node (bf=-1)
	 * 2: successor replaces the old root node
	 * 3: the new bf value becomes -2 (unbalanced)
	 * 4: balance by rotating right
	 * 5: new root = 5
	 */
	{
		AVLTree<int> t;
		std::vector<int> insertionsInOrder = {9, 5, 12, -5, 7, 10, -10};
		for (auto i = 0; i < insertionsInOrder.size(); ++i)
		{
			t.insertNode(insertionsInOrder.at(i));
		}
		t.removeNode(9);
		// t.printTree();
		const auto root = t.getRoot();
		const auto leftRoot = root->getLeft();
		const auto rightRoot = root->getRight();
		if (root->getBf() == 0 &&
			root->getData() == 5 &&
			leftRoot->getBf() == -1 &&
			leftRoot->getData() == -5 &&
			rightRoot->getBf() == 0 &&
			rightRoot->getData() == 10)
		{
			std::cout << "[CASE 7] CORRECT";
		}
		else
		{
			std::cout << "[CASE 7] INCORRECT";
		}
		std::cout << "\n";
	}

	/**
	 * @brief [Case 8]:
	 * 1: removing root with successor
	 * 2: causes left-right rotation
	 * 3: new root = 7 (bf=0)
	 */
	{
		AVLTree<int> t;
		std::vector<int> insertionsInOrder = {9, 6, 12, 4, 7, 10, 8};
		for (auto i = 0; i < insertionsInOrder.size(); ++i)
		{
			t.insertNode(insertionsInOrder.at(i));
		}
		t.removeNode(9);
		// t.printTree();
		const auto root = t.getRoot();
		const auto leftRoot = root->getLeft();
		const auto rightRoot = root->getRight();
		if (root->getBf() == 0 &&
			root->getData() == 7 &&
			leftRoot->getBf() == -1 &&
			leftRoot->getData() == 6 &&
			rightRoot->getBf() == 0 &&
			rightRoot->getData() == 10)
		{
			std::cout << "[CASE 8] CORRECT";
		}
		else
		{
			std::cout << "[CASE 8] INCORRECT";
		}
		std::cout << "\n";
	}

	/**
	 * @brief [Case 9]:
	 * 1: removing root with successor
	 * 2: bf of parent of successor becomes 2
	 * 3: left rotation of successor parent
	 * 4: new root = 15
	 */
	{
		AVLTree<int> t;
		std::vector<int> insertionsInOrder = {9, 5, 30, 1, 15, 40, 50};
		for (auto i = 0; i < insertionsInOrder.size(); ++i)
		{
			t.insertNode(insertionsInOrder.at(i));
		}
		t.removeNode(9);
		// t.printTree();
		const auto root = t.getRoot();
		const auto leftRoot = root->getLeft();
		const auto rightRoot = root->getRight();
		if (root->getBf() == 0 &&
			root->getData() == 15 &&
			leftRoot->getBf() == -1 &&
			leftRoot->getData() == 5 &&
			rightRoot->getBf() == 0 &&
			rightRoot->getData() == 40)
		{
			std::cout << "[CASE 9] CORRECT";
		}
		else
		{
			std::cout << "[CASE 9] INCORRECT";
		}
		std::cout << "\n";
	}

	/**
	 * @brief [Case 10]:
	 * 1: removing right node of root
	 * 2: bf of root becomes -2
	 * 3: right rotation of root
	 * 4: new root = 50
	 */
	{
		AVLTree<int> t;
		std::vector<int> insertionsInOrder = {66, 50, 70, 45, 51, 96, 40};
		for (auto i = 0; i < insertionsInOrder.size(); ++i)
		{
			t.insertNode(insertionsInOrder.at(i));
		}
		t.removeNode(70);
		// t.printTree();
		const auto root = t.getRoot();
		const auto leftRoot = root->getLeft();
		const auto rightRoot = root->getRight();
		if (root->getBf() == 0 &&
			root->getData() == 50 &&
			leftRoot->getBf() == -1 &&
			leftRoot->getData() == 45 &&
			rightRoot->getBf() == 0 &&
			rightRoot->getData() == 66)
		{
			std::cout << "[CASE 10] CORRECT";
		}
		else
		{
			std::cout << "[CASE 10] INCORRECT";
		}
		std::cout << "\n";
	}

	/*
	 *[Edge Case] deleting node 35 where parent has bf = -1 and root has bf = 1 ->
	 * right-left double rotation to balance ->
	 * 55 should be new root with bf = 0
	 */
	{
		AVLTree<int> t;
		std::vector<int> insertionsInOrder = {50, 30, 60, 20, 35, 55, 70, 15, 52, 58, 77, 57};
		for (auto i = 0; i < insertionsInOrder.size(); ++i)
		{
			t.insertNode(insertionsInOrder.at(i));
		}
		t.removeNode(35);
		// t.printTree();
		const auto newRoot = t.getRoot();
		const auto _50 = t.searchNode(50);
		const auto _58 = t.searchNode(58);
		const auto _70 = t.searchNode(70);
		if (newRoot->getBf() == 0 &&
			newRoot->getData() == 55 &&
			_50->getBf() == -1 &&
			_50->getParent() == newRoot &&
			_58->getBf() == -1 &&
			_58->getParent() == newRoot->getRight() &&
			_70->getBf() == 1 &&
			_70->getParent() == newRoot->getRight())
		{
			std::cout << "[Edge Case] CORRECT";
		}
		else
		{
			std::cout << "[Edge Case] INCORRECT";
		}
		std::cout << "\n";
	}

	return 0;
}

int main(int argc, char *argv[])
{
	// return testingHashTableWithBenchmark();
	// return testingBinarySearchTree();
	return testAVLTreeDeletionCases();
	// return testAVLTreeSearchCases();
}
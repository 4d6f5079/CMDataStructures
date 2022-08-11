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
#include <array>

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
		std::cout << "[SEARCH CASE 1] CORRECT getThree is found and adheres to the expected values\n";
	}
	else
	{
		std::cout << "[SEARCH CASE 1] INCORRECT getThree does not adhere to the expected values\n";
	}

	const auto ShouldNoExists = t.searchNode(100);
	if (ShouldNoExists == nullptr)
	{
		std::cout << "[SEARCH CASE 2] CORRECT ShouldNoExists node with value 100 is not in the tree.\n";
	}
	else
	{
		std::cout << "[SEARCH CASE 2] INCORRECT ShouldNoExists node with value 100 is found which should not be the case.\n";
	}

	const auto getNineNine = t.searchNode(99);
	if (getNineNine->getData() == 99 &&
		getNineNine->getBf() == 0 &&
		!getNineNine->hasLeft() &&
		!getNineNine->hasRight())
	{
		std::cout << "[SEARCH CASE 3] CORRECT getNineNine is found and adheres to the expected values\n";
	}
	else
	{
		std::cout << "[SEARCH CASE 3] INCORRECT getNineNine does not adhere to the expected values\n";
	}

	return 0;
}

int testAVLTreeInsertionCases()
{
	AVLTree<int> t;

	t.insertNode(50);
	t.insertNode(60);
	t.insertNode(70);

	if (t.getRoot()->getData() == 60)
	{
		std::cout << "[INSERTION CASE 1] CORRECT rotate left";
	}
	else
	{
		std::cout << "[INSERTION CASE 1] INCORRECT rotate left";
	}
	std::cout << "\n";

	t.insertNode(40);
	t.insertNode(30);

	const auto _40 = t.searchNode(40);
	const auto _30 = t.searchNode(30);
	const auto _50 = t.searchNode(50);
	if (_40->getBf() == 0 &&
		_30->getBf() == 0 &&
		_50->getBf() == 0 &&
		_40->getLeft() == _30 &&
		_40->getRight() == _50)
	{
		std::cout << "[INSERTION CASE 2] CORRECT rotate right";
	}
	else
	{
		std::cout << "[INSERTION CASE 2] INCORRECT rotate right";
	}
	std::cout << "\n";

	t.insertNode(80);
	t.insertNode(75);

	const auto _70 = t.searchNode(70);
	const auto _60 = t.searchNode(60);
	const auto _80 = t.searchNode(80);
	const auto _75 = t.searchNode(75);
	if (_70->getBf() == 0 &&
		_80->getBf() == 0 &&
		_75->getBf() == 0 &&
		_75->getLeft() == _70 &&
		_75->getRight() == _80 &&
		_75->getParent() == _60)
	{
		std::cout << "[INSERTION CASE 3] CORRECT rotate right-left";
	}
	else
	{
		std::cout << "[INSERTION CASE 3] INCORRECT rotate right-left";
	}
	std::cout << "\n";

	t.insertNode(20);
	t.insertNode(25);

	const auto _20 = t.searchNode(20);
	const auto _25 = t.searchNode(25);
	const auto _30_1 = t.searchNode(30);
	if (_20->getBf() == 0 &&
		_30_1->getBf() == 0 &&
		_25->getBf() == 0 &&
		_25->getLeft() == _20 &&
		_25->getRight() == _30_1 &&
		_25->getParent() == _40)
	{
		std::cout << "[INSERTION CASE 4] CORRECT rotate left-right";
	}
	else
	{
		std::cout << "[INSERTION CASE 4] INCORRECT rotate left-right";
	}
	std::cout << "\n";

	t.insertNode(15);

	const auto _20_2 = t.searchNode(20);
	const auto _25_2 = t.searchNode(25);
	const auto _30_2 = t.searchNode(30);
	const auto _60_2 = t.searchNode(60);
	const auto _40_2 = t.searchNode(40);
	const auto _50_2 = t.searchNode(50);
	const auto _15 = t.searchNode(15);
	if (_60_2->getBf() == -1 &&
		_25_2->getBf() == 0 &&
		_20_2->getBf() == -1 &&
		_20_2->getLeft() == _15 &&
		_25_2->getLeft() == _20_2 &&
		_25_2->getRight() == _40_2 &&
		_25_2->getParent() == _60_2 &&
		_40_2->getLeft() == _30_2 &&
		_40_2->getRight() == _50_2)
	{
		std::cout << "[INSERTION CASE 5] CORRECT rotate right with children";
	}
	else
	{
		std::cout << "[INSERTION CASE 5] INCORRECT rotate right with children";
	}
	std::cout << "\n";

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
			std::cout << "[DELETION CASE 1] CORRECT";
		}
		else
		{
			std::cout << "[DELETION CASE 1] INCORRECT";
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
			std::cout << "[DELETION CASE 2] CORRECT";
		}
		else
		{
			std::cout << "[DELETION CASE 2] INCORRECT";
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
			std::cout << "[DELETION CASE 3] CORRECT";
		}
		else
		{
			std::cout << "[DELETION CASE 3] INCORRECT";
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
			std::cout << "[DELETION CASE 4] CORRECT";
		}
		else
		{
			std::cout << "[DELETION CASE 4] INCORRECT";
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
			std::cout << "[DELETION CASE 5] CORRECT";
		}
		else
		{
			std::cout << "[DELETION CASE 5] INCORRECT";
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
			std::cout << "[DELETION CASE 6] CORRECT";
		}
		else
		{
			std::cout << "[DELETION CASE 6] INCORRECT";
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
			std::cout << "[DELETION CASE 7] CORRECT";
		}
		else
		{
			std::cout << "[DELETION CASE 7] INCORRECT";
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
			std::cout << "[DELETION CASE 8] CORRECT";
		}
		else
		{
			std::cout << "[DELETION CASE 8] INCORRECT";
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
			std::cout << "[DELETION CASE 9] CORRECT";
		}
		else
		{
			std::cout << "[DELETION CASE 9] INCORRECT";
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
			std::cout << "[DELETION CASE 10] CORRECT";
		}
		else
		{
			std::cout << "[DELETION CASE 10] INCORRECT";
		}
		std::cout << "\n";
	}

	/**
	 * @brief [Case 11]:
	 * 1: removing root 9
	 * 2: node 10 becomes new root (successor)
	 * 3: node 10 position is replaced with right node 13
	 * 4: no rotations should occur
	 */
	{
		AVLTree<int> t;
		std::vector<int> insertionsInOrder = {9, 7, 20, 5, 8, 15, 40, 6, 10, 30, 13};
		for (auto i = 0; i < insertionsInOrder.size(); ++i)
		{
			t.insertNode(insertionsInOrder.at(i));
		}
		t.removeNode(9);
		// t.printTree();
		const auto root = t.getRoot();
		const auto leftRoot = root->getLeft();
		const auto rightRoot = root->getRight();
		const auto newInorderSuccessor = t.findInorderSuccessor(rightRoot);
		if (root->getBf() == 0 &&
			root->getData() == 10 &&
			leftRoot->getBf() == -1 &&
			leftRoot->getData() == 7 &&
			rightRoot->getBf() == 0 &&
			rightRoot->getData() == 20 &&
			newInorderSuccessor->getData() == 13)
		{
			std::cout << "[DELETION CASE 11] CORRECT";
		}
		else
		{
			std::cout << "[DELETION CASE 11] INCORRECT";
		}
		std::cout << "\n";
	}

	/**
	 * @brief [Case 12]:
	 * 1: removing root 9
	 * 2: node 11 becomes new root (successor)
	 * 3: node 11 position is replaced with right node 12
	 * 4: no rotations should occur
	 */
	{
		AVLTree<int> t;
		std::vector<int> insertionsInOrder = {9, 7, 15, 8, 11, 30, 12};
		for (auto i = 0; i < insertionsInOrder.size(); ++i)
		{
			t.insertNode(insertionsInOrder.at(i));
		}
		t.removeNode(9);
		// t.printTree();
		const auto root = t.getRoot();
		const auto leftRoot = root->getLeft();
		const auto rightRoot = root->getRight();
		const auto newInorderSuccessor = t.findInorderSuccessor(rightRoot);
		if (root->getBf() == 0 &&
			root->getData() == 11 &&
			leftRoot->getBf() == 1 &&
			leftRoot->getData() == 7 &&
			rightRoot->getBf() == 0 &&
			rightRoot->getData() == 15 &&
			newInorderSuccessor->getData() == 12)
		{
			std::cout << "[DELETION CASE 12] CORRECT";
		}
		else
		{
			std::cout << "[DELETION CASE 12] INCORRECT";
		}
		std::cout << "\n";
	}

	/**
	 * @brief [Edge Case]:
	 * 1: deleting node 35 where parent has bf = -1 and root has bf = 1
	 * 2: right-left double rotation to balance
	 * 3: 55 should be new root with bf = 0
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
			std::cout << "[DELETION EDGE CASE] CORRECT";
		}
		else
		{
			std::cout << "[DELETION EDGE CASE] INCORRECT";
		}
		std::cout << "\n";
	}

	return 0;
}

int benchmarkAVLTree()
{
	// Constants
	static constexpr std::array<std::size_t, 6> LOOP_ITERATIONS_POPULATION = {10, 1000, 10000, 100000, 1000000, 2000000};
	static constexpr auto UNIFORM_DIST_RND_MIN = -400000;
	static constexpr auto UNIFORM_DIST_RND_MAX = 500000;

	std::random_device rd;																  // obtain a random number from hardware
	std::mt19937 gen(rd());																  // seed the generator
	std::uniform_int_distribution<int> distr(UNIFORM_DIST_RND_MIN, UNIFORM_DIST_RND_MAX); // define the range

	for (std::size_t insertionIters = 0; insertionIters < LOOP_ITERATIONS_POPULATION.size(); ++insertionIters)
	{
		{
			std::cout << "insertion with iters = " << LOOP_ITERATIONS_POPULATION[insertionIters] << "\n";

			AVLTree<int> tree;
			Timer t;
			for (std::size_t i = 0; i < LOOP_ITERATIONS_POPULATION[insertionIters]; ++i)
			{
				tree.insertNode(distr(gen));
			}
		}
	}

	for (std::size_t deletionIters = 0; deletionIters < LOOP_ITERATIONS_POPULATION.size(); ++deletionIters)
	{
		{
			std::vector<int> insertedData;
			insertedData.clear();
			insertedData.reserve(LOOP_ITERATIONS_POPULATION[deletionIters]);
			AVLTree<int> tree;

			for (std::size_t i = 0; i < LOOP_ITERATIONS_POPULATION[deletionIters]; ++i)
			{
				const auto rand_num = distr(gen);
				tree.insertNode(rand_num);
				insertedData.emplace_back(rand_num);
			}

			// std::random_device del_rd;														  // obtain a random number from hardware
			// std::mt19937 del_gen(del_rd());													  // seed the generator
			// std::uniform_int_distribution<std::size_t> del_distr(0, insertedData.size() - 1); // define the range

			std::cout << "removal with iters = " << LOOP_ITERATIONS_POPULATION[deletionIters] << "\n";
			Timer t;
			for (std::size_t k = 0; k < insertedData.size(); ++k)
			{
				tree.removeNode(insertedData[k]);
			}
		}
	}

	return 0;
}

int main(int argc, char *argv[])
{
	// return testingHashTableWithBenchmark();
	// return testingBinarySearchTree();
	// testAVLTreeSearchCases();
	// testAVLTreeInsertionCases();
	// return testAVLTreeDeletionCases();
	return benchmarkAVLTree();
}
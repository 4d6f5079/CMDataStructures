#include "HashTable.h"
#include "LinkedList.h"
#include "Node.h"
#include <random>
#include <iostream>
#include <functional>

std::string randomStrGen(int length, int rndNum)
{
	const static std::string charset = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ1234567890";
	std::string result;
	result.resize(length);

	for (int i = 0; i < length; i++)
		result[i] = charset[rndNum % charset.length()];

	return result;
}

int main(int argc, char* argv[])
{
	std::default_random_engine generator;
	std::uniform_int_distribution<int32_t> distribution(0, 1000000);
	auto roll_dice = std::bind(distribution, generator);
	auto getRandomString = std::bind(&randomStrGen, std::placeholders::_1, std::placeholders::_2);
	
	//{
		/*LinkedList<std::string> ll(std::string("Hallo"));
		ll.insertAtHead(std::string("Yo"));
		ll.insertAtHead(std::string("No"));
		ll.deleteAtHead();
		ll.printNodes();*/
		//Node<std::string>* test = new Node<std::string>(std::string("test"), &kk);
		//ll.insertNode(test);
		//std::cout << ll.getNode(std::string("Yo")) << std::endl;
	//}

	HashTable<std::string, int32_t> ht(size_t(30));
	static constexpr auto LOOP_ITERATIONS_POPULATION = 2000000;
	std::string k;

	for (size_t i = 0; i < LOOP_ITERATIONS_POPULATION; ++i)
	{
		const auto newValue = roll_dice();
		const auto newKey = getRandomString(25, roll_dice());
		if (i == 0)
		{
			k = newKey;
		}
		ht.put(newKey, newValue);
	}
	ht.get(k).printNodes();
	ht.printBinsInfo();
	return 0;
}
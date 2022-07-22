#pragma once
#include <cstddef>
#include <iostream>

template<typename T>
class AVLNode
{
public:
	
	explicit AVLNode(
		const T& data
	)
		:
		data(data),
		left(nullptr),
		right(nullptr),
		parent(nullptr),
		bf(0) // each node inserted starts off with balance factor of 0 since it is a leaf node (no left or right trees available yet)
	{
	}

	explicit AVLNode(
		const T& data,
		AVLNode* parent,
		signed char bf
	)
		:
		data(data),
		bf(bf),
		parent(parent),
		left(nullptr),
		right(nullptr)
	{

	}
	
	~AVLNode()
	{
		if (!left)
			left = nullptr;
		if (!right)
			right = nullptr;
		if (!parent)
			parent = nullptr;
	}

	const signed char getBf() const
	{
		return bf;
	}

	void setLeft(const AVLNode* newLeft)
	{
		this->left = newLeft;
	}
	
	void setRight(const AVLNode* newRight)
	{
		this->right = newRight;
	}

	void setParent(const AVLNode* newParent)
	{
		this->parent = newParent;
	}

	void setBf(const signed char newHeight)
	{
		this->height = newHeight;
	}

	const T& getData() const
	{
		return data;
	}

	bool operator==(const AVLNode& otherData) const
	{
		return data == otherData.getData();
	}

	bool operator==(AVLNode* otherData) const
	{
		return data == otherData->getData();
	}

	bool hasLeft() const
	{
		if (left != nullptr)
		{
			return true;
		}
		return false;
	}

	bool hasRight() const
	{
		if (right != nullptr)
		{
			return true;
		}
		return false;
	}

	bool hasParent() const
	{
		if (parent != nullptr)
		{
			return true;
		}
		return false;
	}

	AVLNode* getLeft()
	{
		return left;
	}

	AVLNode* getRight()
	{
		return right;
	}

	AVLNode* getParent()
	{
		return parent;
	}

	void printNode()
	{
		std::cout << "\n";
		if (parent)
		{
			std::cout << "	  / \\	     	\n";
			std::cout << "	   |			\n";
			std::cout << "	   |			\n";
			std::cout << parent << "\n";
			std::cout << "	   |			\n";
			std::cout << "	   |			\n";
		}
		std::cout << " ----------------------\n";
		std::cout << "/                      /\n";
		std::cout << "|                      |\n";
		std::cout << "|         " << data << "          |\n";
		std::cout << "|          (bf: " << bf << ")        |\n";
		std::cout << "|     (addr: " << this << ")     |\n";
		std::cout << "\\                      \\\n";
		std::cout << " ----------------------\n";
		if (left)
		{
			std::cout << "LEFT NODE: " << left << "\n\n";
		}
		if (right)
		{
			std::cout << "RIGHT NODE: " << right << "\n\n";
		}
	}

private:
	const T data; // data present in the node
	AVLNode* left; // pointer to left node
	AVLNode* right; // pointer to right node
	AVLNode* parent; // pointer to parent node
	signed char bf; // balance factor of current node 
};
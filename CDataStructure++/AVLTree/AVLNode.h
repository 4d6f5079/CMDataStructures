#pragma once
#include <cstddef>
#include <iostream>

template <typename T>
class AVLNode
{
public:
	explicit AVLNode(
		const T &data)
		: data(data),
		  left(nullptr),
		  right(nullptr),
		  parent(nullptr),
		  bf(0) // each node inserted starts off with balance factor of 0 since it is a leaf node (no left or right trees available yet)
	{
	}

	explicit AVLNode(
		const T &data,
		AVLNode *parent)
		: data(data),
		  parent(parent),
		  bf(0),
		  left(nullptr),
		  right(nullptr)
	{
	}

	// ~AVLNode()
	// {
	// 	if (left)
	// 		left = nullptr;
	// 	if (right)
	// 		right = nullptr;
	// 	if (parent)
	// 		parent = nullptr;
	// }

	inline const signed char getBf() const
	{
		return bf;
	}

	inline void setLeft(AVLNode *newLeft)
	{
		this->left = newLeft;
	}

	inline void setRight(AVLNode *newRight)
	{
		this->right = newRight;
	}

	inline void setParent(AVLNode *newParent)
	{
		this->parent = newParent;
	}

	inline void setBf(const signed char newBf)
	{
		this->bf = newBf;
	}

	inline const T &getData() const
	{
		return data;
	}

	inline bool operator==(const AVLNode &otherData) const
	{
		return data == otherData.getData();
	}

	inline bool operator==(AVLNode *otherData) const
	{
		return data == otherData->getData();
	}

	inline bool hasLeft() const
	{
		if (left != nullptr)
		{
			return true;
		}
		return false;
	}

	inline bool hasRight() const
	{
		if (right != nullptr)
		{
			return true;
		}
		return false;
	}

	inline bool hasParent() const
	{
		if (parent != nullptr)
		{
			return true;
		}
		return false;
	}

	inline AVLNode *getLeft()
	{
		return left;
	}

	inline AVLNode *getRight()
	{
		return right;
	}

	inline AVLNode *getParent()
	{
		return parent;
	}

	inline void printNode()
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
		std::cout << "|          (bf: " << (int)bf << ")        |\n";
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
	const T data;	 // data present in the node
	AVLNode *left;	 // pointer to left node
	AVLNode *right;	 // pointer to right node
	AVLNode *parent; // pointer to parent node
	signed char bf;	 // balance factor of current node
};
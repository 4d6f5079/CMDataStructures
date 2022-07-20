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
		height(0) // each node inserted starts off with balance factor of 0 since it is a leaf node (no left or right trees available yet)
	{
	}

	explicit AVLNode(
		const T& data,
		AVLNode* parent,
		std::size_t height
	)
		:
		data(data),
		height(height),
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

	const std::size_t getHeight() const
	{
		return height;
	}

	void setLeft(AVLNode* newLeft)
	{
		this->left = newLeft;
	}
	
	void setRight(AVLNode* newRight)
	{
		this->right = newRight;
	}

	void setParent(AVLNode* newParent)
	{
		this->parent = newParent;
	}

	void setHeight(std::size_t newHeight)
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
		std::cout << "|          " << data << "            |\n";
		std::cout << "|           (" << height << ")           |\n";
		std::cout << "|           (" << this << ")           |\n";
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
	std::size_t height; // height 
};
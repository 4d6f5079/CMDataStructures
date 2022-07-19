#pragma once
#include <cstddef>

template<typename T>
class AVLNode
{
public:
	
	explicit AVLNode(
		const T& data,
		AVLNode* left = nullptr,
		AVLNode* right = nullptr
	)
		:
		data(data),
		left(left),
		right(right),
		height(0) // each node inserted starts off with balance factor of 0 since it is a leaf node (no left or right trees available yet)
	{
	}
	
	~AVLNode()
	{
		if (!left)
			left = nullptr;
		if (!right)
			right = nullptr;
	}

	const signed char getBf() const
	{
		return bf;
	}

	void setLeft(AVLNode* newLeft)
	{
		this->left = newLeft;
	}
	
	void setRight(AVLNode* newRight)
	{
		this->right = newRight;
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

	AVLNode* getLeft()
	{
		return left;
	}

	AVLNode* getRight()
	{
		return right;
	}

private:
	const T data; // data present in the node
	AVLNode* left; // pointer to left node
	AVLNode* right; // pointer to right node
	std::size_t height; // height 
};
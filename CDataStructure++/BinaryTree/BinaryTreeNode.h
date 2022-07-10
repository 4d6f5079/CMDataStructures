#pragma once

template<typename T>
class BinaryTreeNode
{
public:
	BinaryTreeNode()
		:
		data(NULL),
		left(nullptr),
		right(nullptr)
	{
	}

	BinaryTreeNode(const T& data)
		:
		data(data),
		left(nullptr),
		right(nullptr)
	{
	}

	BinaryTreeNode(
		const T& data,
		BinaryTreeNode* left,
		BinaryTreeNode* right
	)
		:
		data(data),
		left(left),
		right(right)
	{
	}

	~BinaryTreeNode()
	{
		if (hasRight())
			right = nullptr;
		if (hasLeft())
			left = nullptr;
	}

	const BinaryTreeNode& getLeft()
	{
		return left;
	}
	
	const BinaryTreeNode& getRight()
	{
		return right;
	}

	bool hasRight()
	{
		if (right != nullptr)
		{
			return true;
		}
		return false;
	}

	const bool hasLeft()
	{
		if (left != nullptr)
		{
			return true;
		}
		return false;
	}

	const bool hasData()
	{
		if (data != NULL)
		{
			return true;
		}
		return false;
	}

	const T& getData()
	{
		return data;
	}

private:
	const T data;
	BinaryTreeNode* left;
	BinaryTreeNode* right;
};
#pragma once

template<typename T>
class BinarySearchTreeNode
{
public:
	explicit BinarySearchTreeNode(
		const T& data,
		BinarySearchTreeNode* left = nullptr,
		BinarySearchTreeNode* right = nullptr
	)
		:
		data(data),
		left(left),
		right(right)
	{
	}

	~BinarySearchTreeNode()
	{
		if (hasRight())
			right = nullptr;
		if (hasLeft())
			left = nullptr;
	}

	BinarySearchTreeNode* getLeft()
	{
		return left;
	}
	
	BinarySearchTreeNode* getRight()
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

	bool hasLeft()
	{
		if (left != nullptr)
		{
			return true;
		}
		return false;
	}

	const T& getData()
	{
		return data;
	}

	void setLeft(BinarySearchTreeNode* newLeft)
	{
		this->left = newLeft;
	}

	void setRight(BinarySearchTreeNode* newRight)
	{
		this->right = newRight;
	}

private:
	const T data;
	BinarySearchTreeNode* left;
	BinarySearchTreeNode* right;
};
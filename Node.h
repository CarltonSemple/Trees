#pragma once

#include <memory>

template <typename T>
class Node
{
public:
	Node()
	{
	}

	Node(T newValue)
	{
		this->value = newValue;
	}

	~Node()
	{
	}

	T getValue() { return value; }

	// get height recursively
	int height_r();
	int height();
	// how many nodes from this one until there's one with no parent
	int distanceFromTop()
	{
		int count = 0;
		Node<T> * current = this;
		while (current->parent != nullptr)
		{
			current = current->parent;
			count++;
		}
		return count;
	}

	// true if either of the child nodes is 2 greater in height than the other
	// return: 1=left greater, 2=right greater
	int isImbalanced();

	// Determines which child is greater in height
	// return: 1=left greater, 2=right greater
	int greaterSide();

	// balance the nodes this node is immediately responsible for
	// change the originating node's parent's child nodes (change the root) if necessary
	void balance();

	T value;
	Node<T> * parent;
	Node<T> * leftChild;
	Node<T> * rightChild;


private:
	// Root's left child becomes left child's right child
	// left child's right child becomes root node
	// Root's parent's new child is the root's old left child
	// Root's parent becomes root's old left child
	// Left child's parent becomes root's previous parent
	void swapWithLeftChild();

	void swapWithRightChild();

	static void swapNodes(Node<T> * one, Node<T> * two)
	{
		Node<T> * temp = one;
		one = two;
		two = temp;
	}
};

template <typename T>
int Node<T>::height_r()
{
	if (leftChild != nullptr && rightChild != nullptr)
	{
		int leftHeight = leftChild->height_r(), rightHeight = rightChild->height_r();
		if (leftHeight > rightHeight)
			return leftHeight + 1;
		else
			return rightHeight + 1;
	}
	else if (leftChild != nullptr)
	{
		return leftChild->height_r() + 1;
	}
	else if (rightChild != nullptr)
	{
		return rightChild->height_r() + 1;
	}
	else // leaf node, no children
		return 1;
}

template <typename T>
int Node<T>::height()
{

}

template <typename T>
int Node<T>::isImbalanced()
{
	// if both children exist
	// see if one is greater than the other by 2 or more
	if (leftChild != nullptr && rightChild != nullptr)
	{
		int leftHeight = leftChild->height_r(), rightHeight = rightChild->height_r();
		if (leftHeight >= (rightHeight + 2))
			return 1;
		if (rightHeight >= (leftHeight + 2))
			return 2;
	}

	// if 1 child exists and it has children
	else if (leftChild != nullptr)
	{
		if (leftChild->height_r() > 1)
			return 1;
	}
	else if (rightChild != nullptr)
	{
		if (rightChild->height_r() > 1)
			return 2;
	}
	
	return 0;
}

template <typename T>
int Node<T>::greaterSide()
{
	// if both children exist
	if (leftChild != nullptr && rightChild != nullptr)
	{
		int leftHeight = leftChild->height_r(), rightHeight = rightChild->height_r();
		if (leftHeight > rightHeight)
			return 1;
		else if (leftHeight < rightHeight)
			return 2;
		else
			return 0; // they're equal in height
	}

	// if only 1 child exists
	if (leftChild != nullptr)
		return 1;
	if (rightChild != nullptr)
		return 2;

	// neither child exists
	return 0;
}

template <typename T>
void Node<T>::balance()
{
	int imbalanced = isImbalanced();

	// do nothing if already balanced
	if (imbalanced == 0)
		return;
	
	if (imbalanced == 1) // left is greater
	{	
		int leftGreaterSide = leftChild->greaterSide();
		if (leftGreaterSide == 2)
		{	// left child's right child is greater (LR case)
			leftChild->swapWithRightChild();
			// leftChild is now the old left child's rightChild
			this->swapWithLeftChild();
		}
		else // if (leftGreaterSide == 1)
		{	// left child's left child is greater (LL case)
			this->swapWithLeftChild();
		}
	}
	else if (imbalanced == 2) // right is greater
	{
		int rightGreaterSide = rightChild->greaterSide();
		if (rightGreaterSide == 1)
		{	// right child's left child is greater (RL case)
			rightChild->swapWithLeftChild();
			this->swapWithRightChild();
		}
		else // if (rightGreaterSide == )
		{
			// (RR case)
			this->swapWithRightChild();
		}
	}
}

template <typename T>
void Node<T>::swapWithLeftChild()
{
	// left child mustn't be null
	if (leftChild == nullptr)
		return;

	Node<T> * left = leftChild;	// hold on to left child
	
	// make this node's parent point to this node's left child
	if (parent != nullptr)
	{
		if (parent->leftChild == this)
			parent->leftChild = left;
		else if (parent->rightChild == this)
			parent->rightChild = left;
	}

	// this node's leftChild becomes left's right child
	this->leftChild = left->rightChild;
	// fix that child's parent to point to this node
	if(this->leftChild != nullptr)
		this->leftChild->parent = this;

	// left's rightChild becomes this node
	left->rightChild = this;

	// change parents
	left->parent = this->parent;
	this->parent = left;
}

template <typename T>
void Node<T>::swapWithRightChild()
{
	// right child mustn't be null
	if (rightChild == nullptr)
		return;

	Node<T> * right = rightChild; // hold on to right child

	// make this node's parent point to this node's right child
	if (parent != nullptr)
	{
		if (parent->leftChild == this)
			parent->leftChild = right;
		else if (parent->rightChild == this)
			parent->rightChild = right;
	}

	// this node's rightChild becomes right's left child
	this->rightChild = right->leftChild;
	// fix that child's parent to point to this node
	if(this->rightChild != nullptr)
		this->rightChild->parent = this;

	// right's leftChild becomes this node
	right->leftChild = this;

	// change parents
	right->parent = this->parent;
	this->parent = right;
}
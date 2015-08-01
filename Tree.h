#pragma once

#include "Node.h"
#include <queue>

template <typename T>
class BinaryTree
{
public:
	BinaryTree();
	~BinaryTree();

	/* Insert iteratively */
	void insert(T value);
	void insert(Node<T> * newNode);
	
	void print();

private:
	Node<T> * root;
	// keep the root node at the true top of the tree
	void adjustRoot()
	{
		while (root->parent != nullptr)
		{
			root = root->parent;
		}
	}

	// visit each node and call its balance function, starting at the bottom of the tree
	void balanceTree(Node<T> * currentNode);
	void rotate();
	void heightOfNode(Node<T> * topNode);

};

template <typename T>
BinaryTree<T>::BinaryTree()
{
}

template <typename T>
BinaryTree<T>::~BinaryTree()
{
}

template <typename T>
void BinaryTree<T>::insert(T value)
{
	Node<T> * newNode = new Node<T>(value);
	insert(newNode);
}

/* Insert iteratively */
template <typename T>
void BinaryTree<T>::insert(Node<T> * newNode)
{
	if (newNode == nullptr)
		return;
	if (root == nullptr)
	{
		root = newNode;
		return;
	}
	Node<T> * current = this->root;
	while (true)
	{
		if (newNode->getValue() < current->getValue())
		{
			// go left or insert as left child
			if (current->leftChild != nullptr)
				current = current->leftChild;
			else
			{
				current->leftChild = newNode;
				newNode->parent = current;
				balanceTree(root);
				return;
			}
		}
		else // newNode->value is >= current->value
		{
			// go right or insert as right child
			if (current->rightChild != nullptr)
				current = current->rightChild;
			else
			{
				current->rightChild = newNode;
				newNode->parent = current;
				balanceTree(root);
				return;
			}
		}
	}
}

template <typename T>
void BinaryTree<T>::print()
{
	// queue to hold children
	std::queue<Node<T>*> qu;

	Node<T> * current = root;
	int height = current->height_r();

	do {
		// new line if the node is lower in the tree
		if (current->height_r() < height)
		{
			height--;
			cout << endl;
		}
		// print current node's value
		cout << current->value << " ";

		// add the node's children to the queue
		if (current->leftChild != nullptr)
			qu.push(current->leftChild);
		if (current->rightChild != nullptr)
			qu.push(current->rightChild);

		if (qu.size() == 0)
			break;

		// get the child that's been in the queue the longest
		current = qu.front();
		qu.pop();
	} while (true);
}

template <typename T>
void BinaryTree<T>::balanceTree(Node<T> * currentNode)
{
	if (currentNode == nullptr)
		return;
	balanceTree(currentNode->leftChild);
	balanceTree(currentNode->rightChild);
	currentNode->balance();
	adjustRoot();
}

template <typename T>
void BinaryTree<T>::rotate()
{

}

template <typename T>
void BinaryTree<T>::heightOfNode(Node<T> * topNode)
{

}
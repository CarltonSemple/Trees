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
	void remove(T value);
	
	/* Print tree using Breadth-First-Search algorithm */
	void print();

	/* Print in order from lowest to highest */
	void printLowestHighest()
	{
		printInOrder_recursive(root);
	}

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

	Node<T> * getNextHighestNode(Node<T> * startingNode);

	void printInOrder_recursive(Node<T> * currentNode);
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
void BinaryTree<T>::remove(T value)
{
	if (root == nullptr)
		return;
	Node<T> * current = root;
	while (true)
	{
		if (current->value > value)
		{
			current = current->leftChild;
		}
		else if (current->value < value)
		{
			current = current->rightChild;
		}
		else if (current->value == value)
		{
			// value was found
			if (current == root)
			{
				root = nullptr;
			}
			else if (current->leftChild == nullptr && current->rightChild == nullptr)
			{
				// node has no children
				if (current->parent->leftChild == current)
					current->parent->leftChild = nullptr;
				else
					current->parent->rightChild = nullptr;
			}
			else if (current->leftChild == nullptr)
			{
				// node only has right child
				if (current->parent->rightChild == current)
					current->parent->rightChild = current->rightChild;
				else
					current->parent->leftChild = current->rightChild;
				current->rightChild->parent = current->parent;
			}
			else if (current->rightChild == nullptr)
			{
				// node only has left child
				if (current->parent->rightChild == current)
					current->parent->rightChild = current->leftChild;
				else
					current->parent->leftChild = current->leftChild;
				current->leftChild->parent = current->parent;
			}
			else
			{
				// node has both children
				// get the next largest node
				Node<T> * nextLargest = current->rightChild;
				if (nextLargest->leftChild != nullptr)
				{
					while (nextLargest->leftChild != nullptr)
					{
						nextLargest = nextLargest->leftChild;
					}
					nextLargest->parent->leftChild = nullptr;
				}
				if (current->parent->rightChild == current)
					current->parent->rightChild = nextLargest;
				else
					current->parent->leftChild = nextLargest;
				nextLargest->parent = current->parent;
			}
			delete current;
			balanceTree(root);
			return;
		}
	}
}

template <typename T>
void BinaryTree<T>::print()
{
	// queue to hold children
	std::queue<Node<T>*> qu;

	Node<T> * current = root;
	int height = current->distanceFromTop();

	do {
		// new line if the node is lower in the tree
		if (current->distanceFromTop() > height)
		{
			height++;
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
		{
			cout << endl;
			break;
		}

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
Node<T> * BinaryTree<T>::getNextHighestNode(Node<T> * startingNode)
{
	Node<T> * current = startingNode;
	// case where node has right children
	if (current->rightChild != nullptr)
	{
		current = current->rightChild;
		// get the smallest node in the branch
		while (current->leftChild != nullptr)
		{
			current = current->leftChild;
		}
		return current;
	}

	// if current node is a left child (with no right children of its own), 
	// its parent is the next largest node
	if (current->parent != nullptr)
	{
		if (current->parent->leftChild == current)
		{
			return current->parent;
		}
	}

	return nullptr;
}

template <typename T>
void BinaryTree<T>::printInOrder_recursive(Node<T> * currentNode)
{
	if (currentNode == nullptr)
		return;
	printInOrder_recursive(currentNode->leftChild);
	cout << currentNode->value << " ";
	printInOrder_recursive(currentNode->rightChild);
}
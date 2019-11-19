/*
 * BTree.cpp
 * Name: Mike Baldwin
 * Course: EECS 2510 Non-Linear Data Structures - Fall 2019
 * Date: 11/24/2019
 * Description: Implementation of the B-tree
*/

#include <string.h>
#include <iostream>
#include <assert.h>
#include "BTree.h"

BTree::BTree() : root{ nullptr }, keyComparisonCount{ 0 }, bfChangeCount{ 0 }
{
}

void BTree::Insert(const char* x)
{
}

void BTree::Search(const char* cArray)
{
	/*
	 * Searches in the entire tree for a string and prints to console if found (or if not found)
	 * Parameter<str> The string to search for
	*/

	Node* node = Find(cArray);
	if (node != nullptr)
		std::cout << node->data << ' ' << node->count << std::endl;
	else
		std::cout << cArray << " 0" << std::endl;
}

int BTree::GetHeight()
{
	return ComputeHeight(root);
}

int BTree::GetApproxWorkDone()
{
	return keyComparisonCount + bfChangeCount;
}

int BTree::GetUnique()
{
	return TraverseUnique(root);
}

int BTree::GetNonUnique()
{
	return TraverseNonUnique(root);
}

BTree::Node* BTree::Find(const char* cArray)
{
	/*
	 * Used for searching the tree when you want to get a node pointer back as opposed to just a string.
	 * Parameter<str> The string to search for
	 * Returns the node that contains the supplied string, or NULL if not found
	*/

	return nullptr;
}

int BTree::TraverseNonUnique(Node* node)
{
	/*
	 * Traverses the tree (or subtree), calls Traversal on it's children, and returns the count of nodes.
	 * Parameter<node> The node to start the traversal from (in this subtree)
	*/

	return 0;
}

int BTree::TraverseUnique(Node* node)
{
	/*
	 * Traverses the tree (or subtree), calls Traversal on it's children, and returns the count of nodes that have multiple items.
	 * Parameter<node> The node to start the traversal from (in this subtree)
	*/

	return 0;
}

int BTree::ComputeHeight(Node* node)
{
	/*
	 * Trivial recursive traversal function. It calculates the height.
	*/

	return 0;
}

BTree::Node::Node(const char* name, const int count, Node* left, Node* right, int balanceFactor)
{
	std::strcpy(this->data, name);
	this->count = count;
	this->left = left;
	this->right = right;
	this->bf = balanceFactor;
}

bool BTree::Node::IsLeaf()
{
	return left == nullptr && right == nullptr;
}
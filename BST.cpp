/*
 * BST.cpp
 * Name: Mike Baldwin
 * Course: EECS 2510 Non-Linear Data Structures - Fall 2019
 * Date: 11/9/2019
 * Description: Implementation of the Binary Search tree
*/

#include <cstring>
#include <iostream>
#include "BST.h"

BST::BST() : root{ nullptr }, readCount{ 0 }, writeCount{ 0 }
{
}

void BST::Insert(const char* cArray)
{
	/*
	 *	Inserts a string into the tree. If the string already exists, then it will increment it's count.
	 *	Parameter:
	 *		<cArray> This is the string to insert into the tree
	*/

	// If the tree is empty, then we add this node as the root
	if (root == nullptr)
	{
		writeCount++;
		Node* newNode = new Node(cArray, 1, nullptr, nullptr, nullptr);
		root = newNode;
		return;
	}

	// First, we search if the node already exists, if so, we can skip all this and just increment
	Node* current = Find(cArray);
	if (current != nullptr) // A node already exists
	{
		current->count++;
		return;
	}

	// No node exists yet for this value
	current = root;
	while (current != nullptr)
	{
		readCount++;
		if (std::strcmp(cArray, current->name) < 0) // If less than, move to the left subtree
		{
			if (current->left == nullptr) // No node to the left, add a new one
			{
				writeCount++;
				Node* newNode = new Node(cArray, 1, current, nullptr, nullptr);
				current->left = newNode;
				return;
			}

			// Keep moving through the tree
			current = current->left;
		}
		else if (std::strcmp(cArray, current->name) > 0) // If greater than, move to the right subtree
		{
			if (current->right == nullptr) // No node to the right, add a new one
			{
				writeCount++;
				Node* newNode = new Node(cArray, 1, current, nullptr, nullptr);
				current->right = newNode;
				return;
			}

			// Keep it moving
			current = current->right;
		}
	}
}

//void BST::Insert(const char* cArray)
//{
//	/*
//	 *	Inserts a string into the tree. If the string already exists, then it will increment it's count.
//	 *	Parameter:
//	 *		<cArray> This is the string to insert into the tree
//	*/
//
//	Node *p;
//	Node *q;
//
//	if(root == nullptr)
//	{
//		q = new Node(cArray, 1, nullptr, nullptr, nullptr);
//		root = q;
//		return;
//	}
//
//	p = root;
//	do
//	{
//		if(strcmp(cArray, p->name) == 0)
//		{
//			p->count++;
//			return;
//		}
//		if(strcmp(cArray, p->name) > 0)
//		{
//			if(p->right != nullptr)
//				p = p->right;
//			else
//			{
//				q = new Node(cArray, 1, p, nullptr, nullptr);
//				p->right = q;
//				return;
//			}
//		}
//		if(strcmp(cArray, p->name) < 0)
//		{
//			if(p->left != nullptr)
//				p = p->left;
//			else
//			{
//				q = new Node(cArray, 1, p, nullptr, nullptr);
//				p->left = q;
//				return;
//			}
//		}
//	} while(true);
//}

void BST::Search(const char* cArray)
{
	/*
	 * Searches in the entire tree for a string and prints to console if found (or if not found)
	 * Parameter<str> The string to search for
	*/

	Node* node = Find(cArray);
	if (node != nullptr)
		std::cout << node->name << ' ' << node->count << std::endl;
	else
		std::cout << cArray << " 0" << std::endl;
}

int BST::GetHeight()
{
	/*
	 *	Gets the height of the tree
	*/

	return ComputeHeight(root);
}

int BST::GetReadCount()
{
	/*
	 *	Gets the number of reads
	*/

	return readCount;
}

int BST::GetWriteCount()
{
	/*
	 *	Gets the number of writes
	*/

	return writeCount;
}

int BST::GetUnique()
{
	/*
	 *	Gets the number of nodes in the tree that are unique
	*/

	return TraverseUnique(root);
}

int BST::GetNonUnique()
{
	/*
	 *	Gets the number of nodes in the tree
	*/

	return TraverseNonUnique(root);
}

BST::Node* BST::Find(const char* cArray)
{
	/*
	 * Used for searching the tree when you want to get a node pointer back as opposed to just a string.
	 * Parameter<str> The string to search for
	 * Returns the node that contains the supplied string, or NULL if not found
	*/

	Node* current = root;
	while (current != nullptr) // 'Current' will be NULL if 'root == NULL', which is intended!
	{
		readCount++;
		if (std::strcmp(cArray, current->name) == 0)
			return current;
		readCount++;
		if (std::strcmp(cArray, current->name) < 0)
			current = current->left;
		else
			current = current->right;
	}
	return current;
}

int BST::TraverseUnique(Node* node)
{
	/*
	 * Traverses the tree (or subtree), calls Traversal on it's children, and returns the count of nodes that have multiple items.
	 * Parameter<node> The node to start the traversal from (in this subtree)
	*/

	if (node == nullptr)
		return 0;

	int leftSideUnique = TraverseUnique(node->left);
	int rightSideUnique = TraverseUnique(node->right);
	return leftSideUnique + rightSideUnique + ((node->count > 1) ? 1 : 0);
}

int BST::TraverseNonUnique(Node* node)
{
	/*
	 * Traverses the tree (or subtree), calls Traversal on it's children, and returns the count of nodes.
	 * Parameter<node> The node to start the traversal from (in this subtree)
	*/

	if (node == nullptr)
		return 0;

	int leftSideUnique = TraverseNonUnique(node->left);
	int rightSideUnique = TraverseNonUnique(node->right);
	return leftSideUnique + rightSideUnique + 1;
}

int BST::ComputeHeight(Node* node)
{
	/*
	 * Trivial recursive traversal function. It calculates the height.
	*/

	if (node == nullptr)
		return 0;

	int leftHeight = ComputeHeight(node->left);
	int rightHeight = ComputeHeight(node->right);
	return (leftHeight > rightHeight) ? (leftHeight + 1) : (rightHeight + 1);
}

BST::Node::Node(const char* name, const int count, Node* parent, Node* left, Node* right)
{
	std::strcpy(this->name, name);
	this->count = count;
	this->parent = parent;
	this->left = left;
	this->right = right;
}

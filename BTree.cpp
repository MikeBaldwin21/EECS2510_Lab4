/*
 * BTree.cpp
 * Name: Mike Baldwin
 * Course: EECS 2510 Non-Linear Data Structures - Fall 2019
 * Date: 11/24/2019
 * Description: Implementation of the B-tree
*/

#include <string.h>
#include <iostream>
#include <fstream>
#include <assert.h>
#include "BTree.h"

using namespace std;

BTree::BTree(const char* filePath)
{
	/*
	 * Constructor for the B-Tree.
	 * This function will open the file located at the supplied filePath. If none exists, it will make a new one.
	 * If the file is unable to be opened it will throw the failbit in the tree.
	*/

	this->filePath = filePath;
	fstream file;
	file.open(filePath, ios::binary);
	if (file.fail())
	{
		cout << "Unable to open input file" << endl;
		badBit = true;
	}
	file.close();
}

void BTree::Insert(const char* x)
{
	Node r = root;

	if (root.n == 2 * T - 1)
	{
		Node s = Node();
		root = s;
		s.leaf = false;
		s.n = 0;
		s.c[1] = r;
		SplitChild(s, 1);
		InsertNonFull(s, k);
	}
	else
		InsertNonFull(r, k);
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

void BTree::SplitChild(Node x, int i)
{
	Node z = AllocateNode();
	Node y = x.c[i];

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

bool BTree::BadBit()
{
	return badBit;
}

int BTree::TraverseNonUnique(Node node)
{
	/*
	 * Traverses the tree (or subtree), calls Traversal on it's children, and returns the count of nodes.
	 * Parameter<node> The node to start the traversal from (in this subtree)
	*/

	return 0;
}

int BTree::TraverseUnique(Node node)
{
	/*
	 * Traverses the tree (or subtree), calls Traversal on it's children, and returns the count of nodes that have multiple items.
	 * Parameter<node> The node to start the traversal from (in this subtree)
	*/

	return 0;
}

int BTree::ComputeHeight(Node node)
{
	/*
	 * Trivial recursive traversal function. It calculates the height.
	*/

	return 0;
}
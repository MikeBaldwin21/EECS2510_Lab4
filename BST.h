/*
 * BST.h
 * Name: Mike Baldwin
 * Course: EECS 2510 Non-Linear Data Structures - Fall 2019
 * Date: 11/9/2019
 * Description: Declaration of the Binary Search tree
*/

#pragma once

#include "Tree.h"

class BST : Tree
{
public:
	BST();
	void Insert(const char* cArray);
	void Search(const char* cArray);
	int GetHeight();
	int GetApproxWorkDone();
	int GetUnique();
	int GetNonUnique();

private:

	struct Node
	{
		char name[50]{};
		int count;
		Node* parent;
		Node* left;
		Node* right;
		
		Node(const char* name, const int count, Node* parent, Node* left, Node* right);
		bool IsRoot();
		bool IsLeaf();
	};

	Node* root;
	unsigned int keyComparisonCount;
	unsigned int nodeLinkChangeCount;
	Node* Find(const char* cArray);
	int TraverseNonUnique(Node* node);
	int TraverseUnique(Node* node);
	static int ComputeHeight(Node* node);
};

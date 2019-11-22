/*
 * AVL.h
 * Name: Mike Baldwin
 * Course: EECS 2510 Non-Linear Data Structures - Fall 2019
 * Date: 11/24/2019
 * Description: Declaration of the AVL tree
*/

#pragma once

#include "Tree.h"

class AVL : Tree
{
public:
	AVL();
	void Insert(const char* x);
	void Search(const char* cArray);
	int GetHeight();
	int GetApproxWorkDone();
	int GetUnique();
	int GetNonUnique();

private:

	struct Node
	{
		char data[50]{};
		int count;
		Node* left;
		Node* right;
		int bf;
		
		Node(const char* name, const int count, Node* left, Node* right, int balanceFactor);
	};

	Node* root;
	unsigned int keyComparisonCount;
	unsigned int bfChangeCount;
	Node* Find(const char* cArray);
	int TraverseNonUnique(Node* node);
	int TraverseUnique(Node* node);
	int ComputeHeight(Node* node);
};

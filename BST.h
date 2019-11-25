/*
 * BST.h
 * Name: Mike Baldwin
 * Course: EECS 2510 Non-Linear Data Structures - Fall 2019
 * Date: 11/9/2019
 * Description: Declaration of the Binary Search tree
*/

#pragma once

#include "Constants.h"

class BST
{
public:
	BST();
	void Insert(const char* cArray);
	void Search(const char* cArray);
	int GetHeight();
	int GetReadCount();
	int GetWriteCount();
	int GetUnique();
	int GetNonUnique();

private:

	struct Node
	{
		char name[MAX_DATA_LENGTH]{};
		int count;
		Node* parent;
		Node* left;
		Node* right;
		
		Node(const char* name, const int count, Node* parent, Node* left, Node* right);
	};

	Node* root;
	unsigned int readCount;
	unsigned int writeCount;
	Node* Find(const char* cArray);
	int TraverseNonUnique(Node* node);
	int TraverseUnique(Node* node);
	static int ComputeHeight(Node* node);
};

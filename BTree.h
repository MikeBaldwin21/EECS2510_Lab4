/*
 * BTree.h
 * Name: Mike Baldwin
 * Course: EECS 2510 Non-Linear Data Structures - Fall 2019
 * Date: 11/24/2019
 * Description: Declaration of the B-tree
*/

#pragma once

#include "Tree.h"

constexpr int T = 3;
constexpr int KEY_MIN = T - 1;
constexpr int KEY_MAX = 2 * T - 1;
static_assert(T % 2 != 0, "T must be an odd number!"); // According to Lecture 16 - slide #16 (Last sentence)
static_assert(T >= 2, "T must be greater than 2!");
static_assert(KEY_MIN <= KEY_MAX, "KEY_MIN must be less than or equal to KEY_MAX!");

class BTree : Tree
{
public:
	BTree(const char* filePath);
	void Insert(const char* x);
	void Search(const char* cArray);
	int GetHeight();
	int GetApproxWorkDone();
	int GetUnique();
	int GetNonUnique();
	bool BadBit();

private:

	struct Node
	{
		int recordId;
		int n;
		int c[T]; // Array of numbers that point to the correct "record" in the B-Tree file
		bool leaf;
		
		//Node(const char* name, const int count, Node* left, Node* right, int isLeaf);
	};

	Node root;
	const char* filePath;
	unsigned int keyComparisonCount;
	unsigned int bfChangeCount;
	int badBit;
	Node AllocateNode();
	void SplitChild(Node x, int i);
	int TraverseNonUnique(Node node);
	int TraverseUnique(Node node);
	int ComputeHeight(Node node);
	void DiskWrite(Node node, int recordId);
	Node DiskRead(int recordId);
};
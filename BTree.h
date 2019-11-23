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
constexpr int MAX_NODES = 3;
constexpr int KEY_LENGTH = 32;
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
		// The number of keys currently stored in the node
		int n;
		// The keys themselves (n keys). Stored in non-decreasing order
		char keys[KEY_MAX][KEY_LENGTH];
		// n + 1 pointers to this nodes children
		int c[KEY_MAX];
		// Boolean value that is TRUE if this node is a leaf, or false if not
		bool leaf;
		// Data stored in the node, this is the "name" of the node
		char *data;
		// Amount of times the data showes in the node
		int count;

		int GetKeyCount();
	};

	Node root;
	Node nodes[MAX_NODES];
	// NodeIndex is used to keep track of which node to replace in the array of loaded nodes
	int nodeIndex;
	const char* filePath;
	unsigned int keyComparisonCount;
	unsigned int bfChangeCount;
	int badBit;
	Node AllocateNode();
	Node Search(Node x, const char* k);
	void SplitChild(Node x, int i);
	int TraverseNonUnique(Node node);
	int TraverseUnique(Node node);
	int ComputeHeight(Node node);
	void DiskWrite(Node node, int recordId);
	Node DiskRead(int recordId);
};
/*
 * BTree.h
 * Name: Mike Baldwin
 * Course: EECS 2510 Non-Linear Data Structures - Fall 2019
 * Date: 11/24/2019
 * Description: Declaration of the B-tree
*/

#pragma once

#include "Tree.h"

constexpr int T = 3; // <=============== DR. THOMAS, ONLY CHANGE THIS!!! ===============\\ <=== Pump these rookie numbers up

constexpr int KEY_MIN = T - 1; // Minimum amount of keys allowed per node
constexpr int KEY_MAX = 2 * T - 1; // Maximum amount of keys allowed per node
constexpr int MIN_CHILDREN = 2; // Minimum amount of children allowed per node
constexpr int MAX_CHILDREN = 2 * T; // Maximum amount of children allowed per node
constexpr int MAX_DATA_LENGTH = 32; // Largest word allocated (based upon PDF info)
static_assert(T % 2 != 0, "T must be an odd number!"); // According to Lecture 16 - slide #16 (Last sentence)
static_assert(T >= 2, "T must be greater than 2!");
static_assert(KEY_MIN <= KEY_MAX, "KEY_MIN must be less than or equal to KEY_MAX!");
static_assert(T >= 2, "T must be greater than 2!");
static_assert(MIN_CHILDREN <= MAX_DATA_LENGTH, "MIN_CHILDREN must be less than or equal to MAX_DATA_LENGTH!");

class BTree : Tree
{
public:
	BTree(const char* filePath);
	void Insert(const char* cArray);
	void Search(const char* cArray);
	int GetHeight();
	int GetApproxWorkDone();
	int GetUnique();
	int GetNonUnique();

private:

	struct Node
	{
		// This is the 'pointer' to this nodes memory location in the file
		int recordId;
		// The number of keys currently stored in the node
		int n;
		// The keys themselves (n keys). Stored in non-decreasing order
		char keys[KEY_MAX][MAX_DATA_LENGTH];
		// n + 1 'recordId' pointers to this nodes children
		int childRecordId[MAX_CHILDREN];
		// Boolean value that is TRUE if this node is a isLeaf, or false if not
		bool isLeaf;
		// Amount of times the data showes in the node
		int count;

		Node();
		Node(int recordId);
	};

	// This is the recordId of the node that is the root of the tree. NOTE: This is NOT always equal to 1!!!
	int rootRecordId;
	// NodeIndex is used to keep track of which node to replace in the array of loaded nodes
	int nodeIndex;
	// The path (including extension) to the on-disk file used to read/write
	const char* filePath;
	// Tree variable that keeps track of which recordId to write next (a sort of incrmenting counter)
	int nextRecordId;
	int readCount;
	int writeCount;
	int AllocateNode();
	int Search(int recordId, const char* cArray);
	void SplitChild(int recordId, int i);
	void InsertNonFull(int recordId, const char* cArray);
	int TraverseNonUnique(int recordId);
	int TraverseUnique(int recordId);
	int ComputeHeight(int recordId);
	Node DiskRead(int recordId);
	void DiskWrite(Node node);
};
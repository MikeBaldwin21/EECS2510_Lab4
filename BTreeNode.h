/*
 * BTreeNode.h
 * Name: Mike Baldwin
 * Course: EECS 2510 Non-Linear Data Structures - Fall 2019
 * Date: 11/24/2019
 * Description: Declaration of the nodes used in the B-tree
*/

#pragma once

#include "Constants.h"

class BTreeNode
{
public:
	// This is the 'pointer' to this nodes memory location in the file
	int recordId;
	// The number of keys currently stored in the node
	int n;
	// The keys themselves (n keys). Stored in non-decreasing order
	char keys[MAX_KEY + 1][MAX_DATA_LENGTH];
	// Amount of times a key shows in the node
	int counts[MAX_KEY + 1];
	// n + 1 'recordId' pointers to this nodes children
	int childRecordId[MAX_CHILDREN + 1];
	// Boolean value that is TRUE if this node is a isLeaf, or false if not
	bool isLeaf;

	BTreeNode();
	BTreeNode(int recordId);
	int GetKeyCount();
};
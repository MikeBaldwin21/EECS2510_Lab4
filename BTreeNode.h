/*
 * BTreeNode.h
 * Name: Mike Baldwin
 * Course: EECS 2510 Non-Linear Data Structures - Fall 2019
 * Date: 11/24/2019
 * Description: Declaration of the nodes used in the B-tree
*/

#pragma once

constexpr int T = 2; // <=============== DR. THOMAS, ONLY CHANGE THIS!!! ===============\\ <=== Pump these rookie numbers up

constexpr int KEY_MIN = T - 1; // Minimum amount of keys allowed per node
constexpr int KEY_MAX = (2 * T - 1); // Maximum amount of keys allowed per node
constexpr int MIN_CHILDREN = T; // Minimum amount of children allowed per node
constexpr int MAX_CHILDREN = (2 * T); // Maximum amount of children allowed per node
constexpr int MAX_DATA_LENGTH = 32; // Largest word allocated (based upon PDF info)
//static_assert(T % 2 != 0, "T must be an odd number!"); // According to Lecture 16 - slide #16 (Last sentence)
static_assert(T >= 2, "T must be greater than 2!");
static_assert(KEY_MIN <= KEY_MAX, "KEY_MIN must be less than or equal to KEY_MAX!");
static_assert(T >= 2, "T must be greater than 2!");
static_assert(MIN_CHILDREN <= MAX_DATA_LENGTH, "MIN_CHILDREN must be less than or equal to MAX_DATA_LENGTH!");

class BTreeNode
{
public:
	// This is the 'pointer' to this nodes memory location in the file
	int recordId;
	// The number of keys currently stored in the node
	int n;
	// The keys themselves (n keys). Stored in non-decreasing order
	char keys[KEY_MAX + 1][MAX_DATA_LENGTH];
	// n + 1 'recordId' pointers to this nodes children
	int childRecordId[MAX_CHILDREN + 1];
	// Boolean value that is TRUE if this node is a isLeaf, or false if not
	bool isLeaf;
	// Amount of times the data showes in the node
	int count;

	BTreeNode();
	BTreeNode(int recordId);
};
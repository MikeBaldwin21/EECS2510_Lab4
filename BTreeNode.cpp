/*
 * BTree.cpp
 * Name: Mike Baldwin
 * Course: EECS 2510 Non-Linear Data Structures - Fall 2019
 * Date: 11/24/2019
 * Description: Implementation of the nodes used in the B-tree
*/

#include <cstring>
#include "BTreeNode.h"

BTreeNode::BTreeNode()
{
	recordId = 0;
	n = 0;
	isLeaf = true;
	count = 1;

	for (int i = 0; i < MAX_KEY; i++)
		memset(keys[i], 0, sizeof(char) * MAX_DATA_LENGTH);
	memset(childRecordId, 0, sizeof(int) * MAX_CHILDREN);

	/*for (int i = 0; i < (2 * T - 1) + 1; i++)
		for (int j = 0; j < 32; j++)
			keys[i][j] = {};
	for (int i = 0; i < (2 * T) + 1; i++)
		childRecordId[i] = 0;*/
}

BTreeNode::BTreeNode(int recordId)
{
	this->recordId = recordId;
	n = 0;
	isLeaf = true;
	count = 1;

	for (int i = 0; i < MAX_KEY; i++)
		memset(keys[i], 0, sizeof(char) * MAX_DATA_LENGTH);
	memset(childRecordId, 0, sizeof(int) * MAX_CHILDREN);
}
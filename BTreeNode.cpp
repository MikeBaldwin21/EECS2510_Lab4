/*
 * BTreeNode.cpp
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
	for (int i = 0; i < MAX_KEY + 1; i++)
		memset(keys[i], 0, MAX_DATA_LENGTH * sizeof(char));
	memset(childRecordId, 0, (MAX_CHILDREN + 1) * sizeof(int));
}

BTreeNode::BTreeNode(int recordId)
{
	this->recordId = recordId;
	n = 0;
	isLeaf = true;
	count = 1;
	for (int i = 0; i < MAX_KEY + 1; i++)
		memset(keys[i], 0, MAX_DATA_LENGTH * sizeof(char));
	memset(childRecordId, 0, (MAX_CHILDREN + 1) * sizeof(int));
}
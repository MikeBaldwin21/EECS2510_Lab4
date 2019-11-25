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
	isLeaf = false;
	for (int i = 0; i < MAX_KEY + 1; i++)
		memset(keys[i], 0, sizeof(keys[i]));
	memset(counts, 0, sizeof(counts));
	memset(childRecordId, 0, sizeof(childRecordId));
}

BTreeNode::BTreeNode(int recordId)
{
	this->recordId = recordId;
	n = 0;
	isLeaf = false;
	for (int i = 0; i < MAX_KEY + 1; i++)
		memset(keys[i], 0, sizeof(keys[i]));
	memset(counts, 0, sizeof(counts));
	memset(childRecordId, 0, sizeof(childRecordId));
}

int BTreeNode::GetKeyCount()
{
	int count = 0;
	for(int i = 0; i < MAX_KEY + 1; i++)
		if(strcmp(keys[i], "") != 0)
			count++;
	return count;
}
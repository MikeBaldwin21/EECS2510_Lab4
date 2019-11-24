/*
 * BTree.h
 * Name: Mike Baldwin
 * Course: EECS 2510 Non-Linear Data Structures - Fall 2019
 * Date: 11/24/2019
 * Description: Declaration of the B-tree
*/

#pragma once

#include "Tree.h"
#include "BTreeNode.h"

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
	BTreeNode AllocateNode();
	int Search(int recordId, const char* cArray);
	void SplitChild(int recordId, int i);
	void InsertNonFull(int recordId, const char* cArray);
	int TraverseNonUnique(int recordId);
	int TraverseUnique(int recordId);
	int ComputeHeight(int recordId);
	BTreeNode DiskRead(int recordId);
	void DiskWrite(BTreeNode node);
};
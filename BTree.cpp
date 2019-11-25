/*
 * BTree.cpp
 * Name: Mike Baldwin
 * Course: EECS 2510 Non-Linear Data Structures - Fall 2019
 * Date: 11/24/2019
 * Description: Implementation of the B-tree
*/

#include <string.h>
#include <iostream>
#include <fstream>
#include <assert.h>
#include <string>
#include "BTree.h"
#include "BTreeNode.h"
#include "Constants.h"

using namespace std;

long GetFileSize(string filePath);

BTree::BTree(const char* filePath)
{
	/*
	 * Constructor for the B-Tree.
	 * This function will open the file located at the supplied filePath. If none exists, it will make a new one.
	 * If the file is unable to be opened it will throw the failbit in the tree.
	*/

	this->filePath = filePath;
	fstream fileStream;
	fileStream.open(filePath, ios::binary | ios::out | ios::trunc); // Need ios::out for the file to be created & ios::trunc to clear old content
	if (fileStream.fail())
	{
		cout << "Unable to open B-Tree data file" << endl;
		assert(false);
	}
	fileStream.close();

	readCount = writeCount = 0;
	rootRecordId = nextRecordId = 1;
	BTreeNode root = AllocateNode();
	root.isLeaf = true;
	root.n = 0;
	DiskWrite(root);
}

void BTree::Insert(const char* cArray)
{
	/*
	 *	Inserts a key into the tree
	 *	See the Cormen text for details
	*/

	BTreeNode r = DiskRead(rootRecordId); // Load the root from memory

	if (r.n == 2 * T - 1)
	{
		BTreeNode s = AllocateNode();
		rootRecordId = s.recordId;
		s.isLeaf = false;
		s.n = 0;
		s.childRecordId[1] = r.recordId;
		DiskWrite(s);
		SplitChild(s.recordId, 1);
		InsertNonFull(s.recordId, cArray);
	}
	else
		InsertNonFull(r.recordId, cArray);
}

void BTree::InsertNonFull(int recordId, const char* cArray)
{
	/*
	 *	Inserts a key into the tree
	 *	See the Cormen text for details
	*/

	BTreeNode x = DiskRead(recordId);
	for (int j = 1; j <= x.n; j++)
	{
		if (strcmp(cArray, x.keys[j]) == 0)
		{
			x.counts[j]++;
			DiskWrite(x);
			return;
		}
	}

	int i = x.n;

	if (x.isLeaf == true)
	{
		int j = i;
		while (i >= 1 && strcmp(cArray, x.keys[i]) < 0)
		{
			memcpy(x.keys[i + 1], x.keys[i], sizeof(x.keys[i]));
			x.counts[i + 1] = x.counts[i];
			memset(x.keys[i], 0, sizeof(x.keys[i]));
			x.counts[i] = 0;
			i--;
		}
		memcpy(x.keys[i + 1], cArray, sizeof(x.keys[i + 1]));
		x.counts[i + 1]++;
		x.n++;
		DiskWrite(x);
	}
	else
	{
		while (i >= 1 && strcmp(cArray, x.keys[i]) < 0)
			i--;
		i++;
		if (x.childRecordId[i] != 0)
		{
			BTreeNode c1 = DiskRead(x.childRecordId[i]);
			if (c1.n == 2 * T - 1)
			{
				SplitChild(x.recordId, i);
				x = DiskRead(x.recordId);
				if (strcmp(cArray, x.keys[i]) > 0)
					i++;
			}
			InsertNonFull(x.childRecordId[i], cArray);
		}
		else
		{
			cout << "Can't read from a 0 record!" << endl;
			assert(false);
		}
	}
}

void BTree::SplitChild(int recordId, int i)
{
	/*
	 *	Splits a full child with a sibling and it's parent
	 *	See the Cormen text for details
	*/

	BTreeNode x = DiskRead(recordId);
	BTreeNode z = AllocateNode();
	BTreeNode y = DiskRead(x.childRecordId[i]);

	z.isLeaf = y.isLeaf;
	z.n = T - 1;
	for (int j = 1; j <= T - 1; j++)
	{
		memcpy(z.keys[j], y.keys[j + T], sizeof(y.keys[j + T]));
		z.counts[j] = y.counts[j + T];
		memset(y.keys[j + T], 0, sizeof(y.keys[j + T]));
		y.counts[j + T] = 0;
	}

	if (y.isLeaf == false)
	{
		for (int j = 1; j <= T; j++)
		{
			z.childRecordId[j] = y.childRecordId[j + T];
			y.childRecordId[j + T] = 0;
		}
	}

	y.n = T - 1;
	for (int j = x.n + 1; j >= i + 1; j--)
		x.childRecordId[j + 1] = x.childRecordId[j];
	x.childRecordId[i + 1] = z.recordId;

	for (int j = x.n; j >= i; j--)
	{
		memcpy(x.keys[j + 1], x.keys[j], sizeof(x.keys[j]));
		x.counts[j + 1] = x.counts[j];
		memset(x.keys[j], 0, sizeof(x.keys[j]));
		x.counts[j] = 0;
	}

	memcpy(x.keys[i], y.keys[T], sizeof(y.keys[T]));
	x.counts[i] = y.counts[T];
	memset(y.keys[T], 0, sizeof(y.keys[T]));
	y.counts[T] = 0;
	x.n++;

	DiskWrite(x);
	DiskWrite(y);
	DiskWrite(z);
}

void BTree::Search(const char* cArray)
{
	/*
	 * Searches in the entire tree for a string and prints to console if found (or if not found)
	 * Parameter<str> The string to search for
	*/

	BTreeNode node = Search(rootRecordId, cArray);
	int childIndex = 0;
	for (int i = 1; i < node.n; i++)
	{
		if (strcmp(node.keys[i], cArray) == 0)
			childIndex = i;
	}
	std::cout << cArray << ' ' << node.counts[childIndex] << std::endl;
}

int BTree::Search(int recordId, const char* cArray)
{
	/*
	 * Searches in the entire tree for a string and prints to console if found (or if not found)
	 * Parameter<str> The string to search for
	*/

	BTreeNode x = DiskRead(recordId);
	int i = 1;
	while (i <= x.n && strcmp(cArray, x.keys[i]) > 0)
		i++;
	if (i <= x.n && strcmp(cArray, x.keys[i]) == 0)
		return x.childRecordId[i];
	else if (x.isLeaf == true)
		return NULL;
	else
	{
		x = DiskRead(x.childRecordId[i]);
		return Search(x.recordId, cArray);
	}
}

BTreeNode BTree::AllocateNode()
{
	/*
	 *	Writes a new node onto a record on the disk and increments the recordId
	*/

	BTreeNode node = BTreeNode(nextRecordId);
	nextRecordId++;
	DiskWrite(node);
	return node;
}

int BTree::GetHeight()
{
	/*
	 *	Gets the height of the tree
	*/

	return ComputeHeight(rootRecordId);
}

int BTree::GetReadCount()
{
	/*
	 *	Gets the number of reads
	*/

	return readCount;
}

int BTree::GetWriteCount()
{
	/*
	 *	Gets the number of writes
	*/

	return writeCount;
}

int BTree::GetUnique()
{
	/*
	 *	Gets the number of unqiue nodes
	*/

	return TraverseUnique(rootRecordId);
}

int BTree::GetNonUnique()
{
	/*
	 *	Gets the number of non-unique nodes
	*/

	return TraverseNonUnique(rootRecordId);
}

int BTree::TraverseUnique(int recordId)
{
	/*
	 * Traverses the tree (or subtree), calls Traversal on it's children, and returns the count of nodes that have multiple items.
	 * Parameter<node> The node to start the traversal from (in this subtree)
	*/

	if (recordId == 0)
		return 0;

	BTreeNode node = DiskRead(recordId);
	int uniqueCount = 0;
	for (int i = 1; i <= MAX_CHILDREN; i++)
	{
		uniqueCount += (node.counts[i] > 1 ? 1 : 0);
		uniqueCount += TraverseUnique(node.childRecordId[i]);
	}
	return uniqueCount;
}

int BTree::TraverseNonUnique(int recordId)
{
	/*
	 * Traverses the tree (or subtree), calls Traversal on it's children, and returns the count of nodes.
	 * Parameter<node> The node to start the traversal from (in this subtree)
	*/

	if (recordId == 0)
		return 0;

	BTreeNode node = DiskRead(recordId);
	int n = node.n;
	int k = node.GetKeyCount();
	int uniqueCount = node.n;
	for (int i = 1; i <= MAX_CHILDREN; i++)
		uniqueCount += TraverseNonUnique(node.childRecordId[i]);
	return uniqueCount;
}

int BTree::ComputeHeight(int recordId)
{
	/*
	 * Trivial recursive traversal function. It calculates the height.
	*/

	if (recordId == 0)
		return 0;

	BTreeNode node = DiskRead(recordId);
	if (node.isLeaf == true)
		return 1;

	int heights[MAX_CHILDREN + 1];
	memset(heights, 0, MAX_CHILDREN * sizeof(int));
	for (int i = 1; i <= node.n; i++)
		if (recordId != 0)
			heights[i] += ComputeHeight(node.childRecordId[i]);
	int maxHeightIndex = 0;
	for (int i = 1; i <= MAX_CHILDREN; i++)
		if (heights[i] != 0 && heights[i] > heights[maxHeightIndex])
			maxHeightIndex = i;;
	return heights[maxHeightIndex];
}

BTreeNode BTree::DiskRead(int recordId)
{
	/*
	 *	Reads a record from disk into a node
	*/

	readCount++;
	ifstream inputStream;
	inputStream.open(filePath, ios::binary);
	if (inputStream.fail())
	{
		cout << "Unable to open b-tree file for disk reads!" << endl;
		assert(false); // Used for debugging purposes (Assert's are stripped on release build)
	}

	// The size of a single node
	BTreeNode node = BTreeNode();
	long nodeSize = sizeof(BTreeNode);
	long fileSize = GetFileSize(filePath);
	long nodeStartPos = nodeSize * (recordId - 1); // Minus 1 because the node at spot 'zero' is NULL

	if (nodeStartPos >= fileSize)
	{
		cout << "ReadPos is past the end of the file!" << endl;
		assert(false); // Used for debugging purposes (Assert's are stripped on release build)
		return BTreeNode();
	}

	inputStream.seekg(nodeStartPos);

	// Read 'recordId'
	inputStream.read(reinterpret_cast<char*>(&node.recordId), sizeof(node.recordId));

	// Check to make sure the record we just pulled is the correct one
	if (node.recordId != recordId)
	{
		cout << "Record pulled from disk doesn't match what we are looking for!" << endl;
		assert(false);
	}

	// Read 'n'
	inputStream.read(reinterpret_cast<char*>(&node.n), sizeof(node.n));

	// Read 'keys'
	for (int i = 0; i < MAX_KEY + 1; i++)
		inputStream.read(reinterpret_cast<char*>(&node.keys[i]), sizeof(node.keys[i]));

	// Read 'counts'
	for (int i = 0; i < MAX_KEY + 1; i++)
		inputStream.read(reinterpret_cast<char*>(&node.counts[i]), sizeof(node.counts[i]));

	// Read 'childRecordId'
	for (int i = 0; i < MAX_CHILDREN + 1; i++)
		inputStream.read(reinterpret_cast<char*>(&node.childRecordId[i]), sizeof(node.childRecordId[i]));

	// Read 'isLeaf'
	inputStream.read(reinterpret_cast<char*>(&node.isLeaf), sizeof(node.isLeaf));

	inputStream.close();
	return node;
}

void BTree::DiskWrite(BTreeNode node)
{
	/*
	 *	Writes a node into a record on the disk
	*/

	assert(node.recordId != 0);
	writeCount++;
	fstream outputStream;
	outputStream.open(filePath, ios::binary | ios::in | ios::out);
	if (outputStream.fail())
	{
		cout << "Unable to open b-tree file for disk writes!" << endl;
		assert(false); // Used for debugging purposes (Assert's are stripped on release build)
	}

	//Grab the size of a single node, this is what we will use to figure out the writing location
	long nodeSize = sizeof(BTreeNode);
	// Size of the B-Tree disk file
	long fileSize = GetFileSize(filePath);
	long nodeStartPos = nodeSize * (node.recordId - 1); // Minus 1 because the node at spot 'zero' is NULL

	if (nodeStartPos < fileSize) // If less than the fileSize, then we are writing to an existing node.
	{
		// First thing we have to do is clear the old data out
		outputStream.seekp(nodeStartPos, ios::beg);
		char* emptyNodeData = new char[nodeSize];
		for (int i = 0; i < nodeSize; i++) emptyNodeData[i] = NULL;
		outputStream.write(emptyNodeData, nodeSize);
		outputStream.flush();
		delete[] emptyNodeData;
	}
	//if (nodeStartPos == fileSize) // If equal to the fileSize, then we are writing a new node.
	//{
	//	// Do nothing as all we have to do is write to a new location
	//}
	else if (nodeStartPos > fileSize) // If greater than the fileSize, then we are writing to a bad spot.
	{
		cout << "WritePos is past the end of the file!" << endl;
		assert(false); // Used for debugging purposes (Assert's are stripped on release build)
		return;
	}

	outputStream.seekp(nodeStartPos, ios::beg);

	// Write 'recordId'
	outputStream.write(reinterpret_cast<const char*>(&node.recordId), sizeof(node.recordId));

	// Write 'n'
	outputStream.write(reinterpret_cast<const char*>(&node.n), sizeof(node.n));

	// Write 'keys'
	for (int i = 0; i < MAX_KEY + 1; i++)
		outputStream.write(reinterpret_cast<const char*>(&node.keys[i]), sizeof(node.keys[i]));

	// Read 'counts'
	for (int i = 0; i < MAX_KEY + 1; i++)
		outputStream.write(reinterpret_cast<char*>(&node.counts[i]), sizeof(node.counts[i]));

	// Write 'childRecordId'
	for (int i = 0; i < MAX_CHILDREN + 1; i++)
		outputStream.write(reinterpret_cast<const char*>(&node.childRecordId[i]), sizeof(node.childRecordId[i]));

	// Write 'isLeaf'
	outputStream.write(reinterpret_cast<const char*>(&node.isLeaf), sizeof(node.isLeaf));

	// There is bound to be some leftover 'padding' bytes at the end, due to how structures are represented in memory, just fill with zero's
	while (outputStream.tellp() < (long long)nodeStartPos + (long long)nodeSize)
		outputStream.put(0);

	outputStream.close();
}

long GetFileSize(string filePath)
{
	/*
	 * Helper function to find the file size of a file (in bytes), using a path to said file
	 */

	ifstream stream(filePath, ios::binary | ios::ate); // Open the stream at the end, in binary
	streamoff size = stream.is_open() ? (int)stream.tellg() : 0; // If it opened correctly, get the head position, else 0
	stream.close(); // Close the file to free resources
	return (long)size; // Return the found size, or zero
}
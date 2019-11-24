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
	BTreeNode r = DiskRead(rootRecordId); // Load the root from memory

	/*for (int j = 1; j < r.n + 1; j++)
	{
		if (strcmp(cArray, r.keys[j]) == 0)
		{
			r.count++;
			DiskWrite(r);
			return;
		}
	}*/

	if (r.n == MAX_KEY)
	{
		BTreeNode s = AllocateNode();
		rootRecordId = s.recordId;
		s.isLeaf = false;
		s.n = 0;
		s.childRecordId[0] = r.recordId;
		DiskWrite(s);
		SplitChild(s.recordId, 0);
		InsertNonFull(s.recordId, cArray);
	}
	else
		InsertNonFull(r.recordId, cArray);
}

void BTree::InsertNonFull(int recordId, const char* cArray)
{
	BTreeNode x = DiskRead(recordId);

	for (int i = 0; i < x.n; i++)
	{
		if (strcmp(cArray, x.keys[i]) == 0)
		{
			x.count++;
			DiskWrite(x);
			return;
		}
	}

	int i = x.n;
	if (x.isLeaf == true)
	{
		int j = i;

		// Move down through the keys searching for the location of the inserting key (key to it's left is less than it, key to it's right is greater than it)
		while (i >= 0 && strcmp(cArray, x.keys[i]) < 0)
		{
			// If this nodes key is greater than the inserting key, shift this nodes key to the right (to make room)
			memcpy(x.keys[i + 1], x.keys[i], sizeof(x.keys[i]));
			memset(x.keys[j], 0, sizeof(x.keys[j])); // Clear the old spot
			i--; // Decrement the counter to compare the next lowest key
		}

		// We shouldof found the spot now, or we are at the start of the keys, so insert here
		memcpy(x.keys[i + 1], cArray, sizeof(x.keys[i + 1]));
		x.n++;
		DiskWrite(x);
	}
	else // if(x.leaf == false)
	{
		// Move down through the keys searching for the location where the key to the left is less than and the key to the right is greater than it
		while (i >= 0 && strcmp(cArray, x.keys[i]) < 0)
			i--;
		i++; // incrementby 1 to get the correct child location
		if (x.childRecordId[i] != 0) // Should always be true if the algorithm is working correctly
		{
			// Load the child node at that location to check it's stats
			BTreeNode c1 = DiskRead(x.childRecordId[i]);

			// If the child node is full, we will split it
			if (c1.n == MAX_KEY)
			{
				SplitChild(x.recordId, i);

				if (strcmp(cArray, x.keys[i]) > 0)
					i++;
			}

			// Insert the new key into the child node (or deeper still)
			InsertNonFull(c1.recordId, cArray);
		}
		else
		{
			cout << "Can't read from a 0 record!" << endl;
			assert(false);
		}
	}
}

void BTree::SplitChild(int parentRecordId, int index)
{
	BTreeNode parent = DiskRead(parentRecordId);
	BTreeNode splittingChild = DiskRead(parent.childRecordId[index]);
	BTreeNode newChild = AllocateNode();

	assert(splittingChild.n == MAX_KEY);

	newChild.isLeaf = splittingChild.isLeaf;
	splittingChild.n = MIN_KEY;
	splittingChild.n = MIN_KEY;

	// Transfer the right children over to the newChild
	for (int i = 0; i < MIN_KEY; i++)
	{
		memcpy(newChild.keys[i], splittingChild.keys[T + i], sizeof(splittingChild.keys[T + i])); // Copy the key over
		memset(splittingChild.keys[T + i], 0, sizeof(splittingChild.keys[T + i])); // Clear the old key
	}

	// Transfer the children over
	for (int i = 0; i < MIN_CHILDREN; i++)
	{
		newChild.childRecordId[i] = splittingChild.childRecordId[T + i]; // Copy the key over
		splittingChild.childRecordId[T + i] = 0; // Clear the old key
	}

	// Shift the parent keys, from the right of the splittingChildIndex, to the right
	for (int i = parent.n; i > index; i--)
	{
		memcpy(parent.keys[i + 1], parent.keys[i], sizeof(parent.keys[i])); // Shift all the keys, from the splitting child location, to the right
		memset(parent.keys[i], 0, sizeof(parent.keys[i])); // Clear the old spot (not really needed, but I like it)
	}
	memcpy(parent.keys[index], splittingChild.keys[MIN_KEY], sizeof(splittingChild.keys[MIN_KEY])); // Copy the middle key form the splitting child up to the parent (in the correct position)
	memset(splittingChild.keys[MIN_KEY], 0, sizeof(splittingChild.keys[MIN_KEY])); // Clear the old key
	parent.n++;

	// Shift the parent childrenRecordIds, from the right of the splittingChildIndex, to the right
	for (int i = parent.n; i > index; i--)
	{
		parent.childRecordId[i + 1] = parent.childRecordId[i]; // Shift all the keys, from the splitting child location, to the right
		parent.childRecordId[i] = 0; // Clear the old spot (not really needed, but I like it)
	}
	parent.childRecordId[index] = newChild.recordId; // Add the newChild node to the parent list of children

	DiskWrite(parent);
	DiskWrite(splittingChild);
	DiskWrite(newChild);
}

//void BTree::SplitChild(int recordId, int i)
//{
//	BTreeNode x = DiskRead(recordId);
//	BTreeNode y = DiskRead(x.childRecordId[i]);
//	BTreeNode z = AllocateNode();
//
//	z.isLeaf = y.isLeaf;
//	z.n = T - 1;
//	for (int j = 1; j <= T - 1; j++)
//	{
//		memcpy(z.keys[j], y.keys[j + T], sizeof(y.keys[j + T]));
//		memset(y.keys[j + T], 0, sizeof(y.keys[j + T]));
//	}
//
//	if (y.isLeaf == false)
//	{
//		for (int j = 1; j <= T; j++)
//		{
//			z.childRecordId[j] = y.childRecordId[j + T];
//			y.childRecordId[j + T] = 0;
//		}
//	}
//
//	y.n = T - 1;
//	for (int j = x.n + 1; j >= i + 1; j--)
//		x.childRecordId[j + 1] = x.childRecordId[j];
//	/*if(x.n + 1 >= i + 1)
//		x.childRecordId[x.n + 1] = 0;*/
//	x.childRecordId[i + 1] = z.recordId;
//
//	for (int j = x.n; j >= i; j--)
//	{
//		memcpy(x.keys[j + 1], x.keys[j], sizeof(x.keys[j]));
//		memset(x.keys[j], 0, sizeof(x.keys[j]));
//	}
//
//	memcpy(x.keys[i], y.keys[T], sizeof(y.keys[T]));
//	memset(y.keys[T], 0, sizeof(y.keys[T]));
//	x.n++;
//
//	DiskWrite(x);
//	DiskWrite(y);
//	DiskWrite(z);
//}

BTreeNode BTree::AllocateNode()
{
	BTreeNode node = BTreeNode(nextRecordId);
	nextRecordId++;
	DiskWrite(node);
	return node;
}

void BTree::Search(const char* cArray)
{
	/*
	 * Searches in the entire tree for a string and prints to console if found (or if not found)
	 * Parameter<str> The string to search for
	*/

	BTreeNode node = Search(rootRecordId, cArray);
	std::cout << cArray << ' ' << node.count << std::endl;
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

int BTree::GetHeight()
{
	return ComputeHeight(rootRecordId);
}

int BTree::GetApproxWorkDone()
{
	return readCount + writeCount;
}

int BTree::GetUnique()
{
	return TraverseUnique(rootRecordId);
}

int BTree::GetNonUnique()
{
	return TraverseNonUnique(rootRecordId);
}

int BTree::TraverseNonUnique(int recordId)
{
	/*
	 * Traverses the tree (or subtree), calls Traversal on it's children, and returns the count of nodes.
	 * Parameter<node> The node to start the traversal from (in this subtree)
	*/

	return 0;
}

int BTree::TraverseUnique(int recordId)
{
	/*
	 * Traverses the tree (or subtree), calls Traversal on it's children, and returns the count of nodes that have multiple items.
	 * Parameter<node> The node to start the traversal from (in this subtree)
	*/

	return 0;
}

int BTree::ComputeHeight(int recordId)
{
	/*
	 * Trivial recursive traversal function. It calculates the height.
	*/

	return 0;
}

BTreeNode BTree::DiskRead(int recordId)
{
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
	for (int i = 0; i < MAX_KEY; i++)
		inputStream.read(reinterpret_cast<char*>(&node.keys[i]), sizeof(node.keys[i]));

	// Read 'childRecordId'
	for (int i = 0; i < MAX_CHILDREN; i++)
		inputStream.read(reinterpret_cast<char*>(&node.childRecordId[i]), sizeof(node.childRecordId[i]));

	// Read 'isLeaf'
	inputStream.read(reinterpret_cast<char*>(&node.isLeaf), sizeof(node.isLeaf));

	// Read 'count'
	inputStream.read(reinterpret_cast<char*>(&node.count), sizeof(node.count));

	inputStream.close();
	return node;
}

void BTree::DiskWrite(BTreeNode node)
{
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
		memset(emptyNodeData, 0, nodeSize);
		//for (int i = 0; i < nodeSize; i++) emptyNodeData[i] = NULL;
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
	for (int i = 0; i < MAX_KEY; i++)
		outputStream.write(reinterpret_cast<const char*>(&node.keys[i]), sizeof(node.keys[i]));

	// Write 'childRecordId'
	for (int i = 0; i < MAX_CHILDREN; i++)
		outputStream.write(reinterpret_cast<const char*>(&node.childRecordId[i]), sizeof(node.childRecordId[i]));

	// Write 'isLeaf'
	outputStream.write(reinterpret_cast<const char*>(&node.isLeaf), sizeof(node.isLeaf));

	// Write 'count'
	outputStream.write(reinterpret_cast<const char*>(&node.count), sizeof(node.count));

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
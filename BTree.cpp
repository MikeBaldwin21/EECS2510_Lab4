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
	Node root = AllocateNode();
	root.isLeaf = true;
	root.n = 0;
	DiskWrite(root);
}

void BTree::Insert(const char* cArray)
{
	Node r = DiskRead(rootRecordId); // Load the root from memory

	/*for (int j = 1; j < r.n + 1; j++)
	{
		if (strcmp(cArray, r.keys[j]) == 0)
		{
			r.count++;
			DiskWrite(r);
			return;
		}
	}*/

	if (r.n == 2 * T - 1)
	{
		Node s = AllocateNode();
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
	Node x = DiskRead(recordId);

	for (int j = 1; j < x.n + 1; j++)
	{
		if (strcmp(cArray, x.keys[j]) == 0)
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
		while (i >= 1 && strcmp(cArray, x.keys[i]) < 0)
		{
			memcpy(x.keys[i + 1], x.keys[i], sizeof(x.keys[i]));
			memset(x.keys[i], 0, sizeof(x.keys[i]));
			i--;
		}
		memcpy(x.keys[i + 1], cArray, sizeof(x.keys[i + 1]));
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
			Node c1 = DiskRead(x.childRecordId[i]);
			if (c1.n == 2 * T - 1)
			{
				SplitChild(x.recordId, i);
				x = DiskRead(recordId);
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
	Node x = DiskRead(recordId);
	Node z = AllocateNode();
	Node y = DiskRead(x.childRecordId[i]);

	z.isLeaf = y.isLeaf;
	z.n = T - 1;
	for (int j = 1; j <= T - 1; j++)
	{
		memcpy(z.keys[j], y.keys[j + T], sizeof(y.keys[j + T]));
		memset(y.keys[j + T], 0, sizeof(y.keys[j + T]));
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
	/*if(x.n + 1 >= i + 1)
		x.childRecordId[x.n + 1] = 0;*/
	x.childRecordId[i + 1] = z.recordId;

	for (int j = x.n; j >= i; j--)
	{
		memcpy(x.keys[j + 1], x.keys[j], sizeof(x.keys[j]));
		memset(x.keys[j], 0, sizeof(x.keys[j]));
	}

	memcpy(x.keys[i], y.keys[T], sizeof(y.keys[T]));
	memset(y.keys[T], 0, sizeof(y.keys[T]));
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

	Node node = Search(rootRecordId, cArray);
	std::cout << cArray << ' ' << node.count << std::endl;
}

int BTree::Search(int recordId, const char* cArray)
{
	/*
	 * Searches in the entire tree for a string and prints to console if found (or if not found)
	 * Parameter<str> The string to search for
	*/

	Node x = DiskRead(recordId);
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

BTree::Node BTree::AllocateNode()
{
	int prevRecordId = nextRecordId;
	Node node = Node(nextRecordId);
	nextRecordId++;
	DiskWrite(node);
	return node;
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

BTree::Node BTree::DiskRead(int recordId)
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
	Node node = Node();
	long nodeSize = sizeof(Node);
	long fileSize = GetFileSize(filePath);
	long nodeStartPos = nodeSize * (recordId - 1); // Minus 1 because the node at spot 'zero' is NULL

	if (nodeStartPos >= fileSize)
	{
		cout << "ReadPos is past the end of the file!" << endl;
		assert(false); // Used for debugging purposes (Assert's are stripped on release build)
		return Node();
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
	for (int i = 0; i < (2 * T - 1) + 1; i++)
		inputStream.read(reinterpret_cast<char*>(&node.keys[i]), sizeof(node.keys[i]));

	// Read 'childRecordId'
	for (int i = 0; i < (2 * T) + 1; i++)
		inputStream.read(reinterpret_cast<char*>(&node.childRecordId[i]), sizeof(node.childRecordId[i]));

	// Read 'isLeaf'
	inputStream.read(reinterpret_cast<char*>(&node.isLeaf), sizeof(node.isLeaf));

	// Read 'count'
	inputStream.read(reinterpret_cast<char*>(&node.count), sizeof(node.count));

	inputStream.close();
	return node;
}

void BTree::DiskWrite(Node node)
{
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
	long nodeSize = sizeof(Node);
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
	for (int i = 0; i < (2 * T - 1) + 1; i++)
		outputStream.write(reinterpret_cast<const char*>(&node.keys[i]), sizeof(node.keys[i]));

	// Write 'childRecordId'
	for (int i = 0; i < (2 * T) + 1; i++)
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

BTree::Node::Node() : recordId{ 0 }, n{ 0 }, keys{}, childRecordId{}, isLeaf{ true }, count{ 1 }
{
	for (int i = 0; i < (2 * T - 1) + 1; i++)
		for (int j = 0; j < 32; j++)
			keys[i][j] = {};
	for (int i = 0; i < (2 * T) + 1; i++)
		childRecordId[i] = 0;
}

BTree::Node::Node(int recordId) : recordId{ recordId }, n{ 0 }, keys{}, childRecordId{}, isLeaf{ true }, count{ 1 }
{
	for (int i = 0; i < (2 * T - 1) + 1; i++)
		for (int j = 0; j < 32; j++)
			keys[i][j] = {};
	for (int i = 0; i < (2 * T) + 1; i++)
		childRecordId[i] = 0;
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
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

streamoff GetFileSize(string filePath);

BTree::BTree(const char* filePath)
{
	/*
	 * Constructor for the B-Tree.
	 * This function will open the file located at the supplied filePath. If none exists, it will make a new one.
	 * If the file is unable to be opened it will throw the failbit in the tree.
	*/

	this->filePath = filePath;
	fstream file;
	file.open(filePath, ios::binary);
	if (file.fail())
	{
		cout << "Unable to open input file" << endl;
		badBit = true;
	}
	file.close();
}

void BTree::Insert(const char* x)
{
	Node r = root;

	if (root.n == 2 * T - 1)
	{
		Node s = Node();
		root = s;
		s.leaf = false;
		s.n = 0;
		s.c[1] = r;
		SplitChild(s, 1);
		InsertNonFull(s, k);
	}
	else
		InsertNonFull(r, k);
}

void BTree::Search(const char* cArray)
{
	/*
	 * Searches in the entire tree for a string and prints to console if found (or if not found)
	 * Parameter<str> The string to search for
	*/

	Node node = Search(root, cArray);
	std::cout << node.data << ' ' << node.count << std::endl;
}

BTree::Node BTree::Search(Node x, const char* k)
{
	/*
	 * Searches in the entire tree for a string and prints to console if found (or if not found)
	 * Parameter<str> The string to search for
	*/

	int i = 1;
	while (i <= x.GetKeyCount() && strcmp(k, x.keys[i]) > 0)
	{
		i++;
	}
	if (i <= x.GetKeyCount() && strcmp(k, x.keys[i]) == 0)
		return x;
	else if (x.leaf == true)
		return nullptr;
	else (x = DiskRead(x.c[i]))
		return Search(x, )
}

void BTree::SplitChild(Node x, int i)
{
	Node z = AllocateNode();
	Node y = x.c[i];

}

int BTree::GetHeight()
{
	return ComputeHeight(root);
}

int BTree::GetApproxWorkDone()
{
	return keyComparisonCount + bfChangeCount;
}

int BTree::GetUnique()
{
	return TraverseUnique(root);
}

int BTree::GetNonUnique()
{
	return TraverseNonUnique(root);
}

bool BTree::BadBit()
{
	return badBit;
}

int BTree::TraverseNonUnique(Node node)
{
	/*
	 * Traverses the tree (or subtree), calls Traversal on it's children, and returns the count of nodes.
	 * Parameter<node> The node to start the traversal from (in this subtree)
	*/

	return 0;
}

int BTree::TraverseUnique(Node node)
{
	/*
	 * Traverses the tree (or subtree), calls Traversal on it's children, and returns the count of nodes that have multiple items.
	 * Parameter<node> The node to start the traversal from (in this subtree)
	*/

	return 0;
}

int BTree::ComputeHeight(Node node)
{
	/*
	 * Trivial recursive traversal function. It calculates the height.
	*/

	return 0;
}

void BTree::DiskWrite(Node node, int recordId)
{
	ofstream outputStream;
	outputStream.open(filePath, ios::binary);
	if (outputStream.fail())
	{
		cout << "Unable to open b-tree file for disk writes" << endl;
		badBit = true;
		assert(badBit == false); // Used for debugging purposes (Assert's are stripped on release build)
	}

	//Grab the size of a single node, this is what we will use to figure out the writing location
	int nodeSize = sizeof(Node);
	long writePos = nodeSize * node.recordId;

	// We need to find out if the file is even large enough for this
	long fileSize = GetFileSize(filePath);
	if(writePos > fileSize) // If less than OR EQUAL to the fileSize, then we are good.
	{
		// We're way to far out, so lets burn through the needed data, filling it up with junk.
	}

	outputStream.seekp(writePos);

	Node node = Node();
	char* intData = new char[4];
	char* strData = new char[KEY_LENGTH];

	// Read 'recordId'
	for (int i = 0; i < 4; i++) intData[i] = 0;
	inputStream.read(intData, 4);
	int tempRecordID = stoi(intData);
	node.recordId = stoi(intData);

	// Read 'n'
	for (int i = 0; i < 4; i++) intData[i] = 0;
	inputStream.read(intData, 4);
	node.n = stoi(intData);

	// Read 'keys'
	for (int i = 0; i < KEY_MAX; i++)
	{
		for (int j = 0; j < KEY_LENGTH; j++) strData[j] = 0;
		inputStream.read(strData, KEY_LENGTH);
		/*for(int k = 0; k < KEY_LENGTH; k++)
			node.keys[i][k] = data[k];*/
		strcpy(node.keys[i], strData);
	}

	// Read 'c'
	for (int i = 0; i < KEY_MAX; i++)
	{
		for (int j = 0; j < 4; j++) intData[j] = 0;
		inputStream.read(intData, 4);
		node.c[i] = stoi(intData);
	}

	// Read 'leaf'
	for (int i = 0; i < 4; i++) intData[i] = 0;
	inputStream.read(intData, 1);
	node.leaf = stoi(intData);

	// Read 'data'
	for (int i = 0; i < KEY_LENGTH; i++) strData[i] = 0;
	inputStream.read(strData, KEY_LENGTH);
	strcpy(node.data, strData);

	// Read 'count'
	for (int i = 0; i < 4; i++) intData[i] = 0;
	inputStream.read(intData, 4);
	node.count = stoi(intData);
	
	delete[] strData;
	delete[] intData;
	inputStream.close();
	return node;


	output.close();
}

BTree::Node BTree::DiskRead(int recordId)
{
	// NOTE FOR FUTURE MIKE!!!
	// No need to 'search' for a record position. It is guarenteed to always be at the specified position because we are doing NO DELETIONS!!!!

	ifstream inputStream;
	inputStream.open(filePath, ios::binary);
	if (inputStream.fail())
	{
		cout << "Unable to open b-tree file for disk reads" << endl;
		badBit = true;
		assert(badBit == false); // Used for debugging purposes (Assert's are stripped on release build)
	}

	// The size of a single node, this is out 'stepping' amount while we search for our node
	int nodeSize = sizeof(Node);
	long fileSize = GetFileSize(filePath);

	while(inputStream.tellg() < fileSize)
	{
		inputS
		inputStream.seekg(nodeSize, ios_base::_Seekcur);
	}

	Node node = Node();
	char* intData = new char[4];
	char* strData = new char[KEY_LENGTH];

	// Read 'recordId'
	for (int i = 0; i < 4; i++) intData[i] = 0;
	inputStream.read(intData, 4);
	int tempRecordID = stoi(intData);
	node.recordId = stoi(intData);

	// Read 'n'
	for (int i = 0; i < 4; i++) intData[i] = 0;
	inputStream.read(intData, 4);
	node.n = stoi(intData);

	// Read 'keys'
	for (int i = 0; i < KEY_MAX; i++)
	{
		for (int j = 0; j < KEY_LENGTH; j++) strData[j] = 0;
		inputStream.read(strData, KEY_LENGTH);
		/*for(int k = 0; k < KEY_LENGTH; k++)
			node.keys[i][k] = data[k];*/
		strcpy(node.keys[i], strData);
	}

	// Read 'c'
	for (int i = 0; i < KEY_MAX; i++)
	{
		for (int j = 0; j < 4; j++) intData[j] = 0;
		inputStream.read(intData, 4);
		node.c[i] = stoi(intData);
	}

	// Read 'leaf'
	for (int i = 0; i < 4; i++) intData[i] = 0;
	inputStream.read(intData, 1);
	node.leaf = stoi(intData);

	// Read 'data'
	for (int i = 0; i < KEY_LENGTH; i++) strData[i] = 0;
	inputStream.read(strData, KEY_LENGTH);
	strcpy(node.data, strData);

	// Read 'count'
	for (int i = 0; i < 4; i++) intData[i] = 0;
	inputStream.read(intData, 4);
	node.count = stoi(intData);
	
	delete[] strData;
	delete[] intData;
	inputStream.close();
	return node;
}

int BTree::Node::GetKeyCount()
{
	int temp = 0;
	for (int i = 0; i < KEY_MAX; i++)
		if (strcmp(keys[i], "0") != 0)
			temp++;
	return temp;
}

streamoff GetFileSize(string filePath)
{
	/*
	 * Helper function to find the file size of a file (in bytes), using a path to said file
	 */

	ifstream stream(filePath, ios::binary | ios::ate); // Open the stream at the end, in binary
	streamoff size = stream.is_open() ? (int)stream.tellg() : 0; // If it opened correctly, get the head position, else 0
	stream.close(); // Close the file to free resources
	return size; // Return the found size, or zero
}
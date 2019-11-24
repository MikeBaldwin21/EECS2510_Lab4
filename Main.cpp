/*
 * File Name: Main.cpp
 * Date: 11/24/2019
 * Course: EECS 2510 Non-Linear Data Structures
 * Author: Mike Baldwin
 * Brief Description: This file contains the command line parsing logic.
*/

#include <time.h>
#include <cstdio>
#include <string.h>
#include <iostream>
#include <fstream>
#include "Tree.h"
#include "BST.h"
#include "AVL.h"
#include "BTree.h"

using namespace std;

bool RunCustomCommands();
void RunLoopInsert();
void RunLoopInsertOnBTree();
void RunInsertFromFile(const char* filePath, int treeNum);
void RunBTreeInsertFromFile(const char* filePath);

int main(int argc, char* argv[])
{
	RunBTreeInsertFromFile("Docs/Hamlet-Scene-1.txt");
	/*char command[50] = "";

	while (strcmp(command, "exit") != 0)
	{
		cout << endl << "Please enter in '1' for custom commands, '2' for Listing1(for(8200)), or '3' for Listing2(shakespeare.txt): ";
		cin >> command;

		if (strcmp(command, "1") == 0)
			if (RunCustomCommands())
				return 0;
		if (strcmp(command, "2") == 0)
			RunLoopInsert();
		if (strcmp(command, "3") == 0)
		{
			for (int i = 0; i < 5; i++)
				RunInsertFromFile("Docs/Shakespeare.txt", i);
		}
		if (strcmp(command, "4") == 0)
		{
			RunLoopInsertOnBTree();
		}
		if (strcmp(command, "5") == 0)
		{
			RunBTreeInsertFromFile("Docs/Hamlet-Scene-1.txt");
		}
	}*/
	return 0;
}

bool RunCustomCommands()
{
	BST bstTree = BST();
	AVL avlTree = AVL();
	BTree bTree = BTree("TestDisk.txt");
	char command[50] = "";

	while (strcmp(command, "return") != 0)
	{
		cin >> command;

		if (strlen(command) > 0)
		{
			if (strcmp(command, "insert") == 0)
			{
				char str2[50];
				cin >> str2;
				bstTree.Insert(str2);
				avlTree.Insert(str2);
				bTree.Insert(str2);
			}
			else if (strcmp(command, "search") == 0)
			{
				char str2[50];
				cin >> str2;
				bstTree.Search(str2);
				avlTree.Search(str2);
				bTree.Search(str2);
			}
			else if (strcmp(command, "exit") == 0)
			{
				return true;
			}
		}
	}
	return false;
}

void RunLoopInsert()
{
	BST bstTree = BST();
	AVL avlTree = AVL();
	BTree bTree = BTree("TestDisk.txt");

	char chari[10];
	for (int i = 0; i < 10; i++) chari[i] = '\0';

	clock_t start = clock();
	for (int i = 1001; i <= 9200; i++) // insert 8200 strings in BST
	{
		sprintf(chari, "%4i", i);
		bstTree.Insert(chari);
		cout << "Tree Height is now " << bstTree.GetHeight() << endl;
	}
	double BSTtime = ((double)(clock() - (double)start)) / CLOCKS_PER_SEC;
	start = clock();
	for (int i = 1001; i <= 9200; i++) // insert 8200 strings in AVL Tree
	{
		sprintf(chari, "%4i", i);
		avlTree.Insert(chari);
		cout << "Tree Height is now " << avlTree.GetHeight() << endl;
	}
	double AVLtime = ((double)(clock() - (double)start)) / CLOCKS_PER_SEC;
	start = clock();
	for (int i = 1001; i <= 9200; i++) // insert 8200 strings in BTree
	{
		sprintf(chari, "%4i", i);
		bTree.Insert(chari);
		cout << "Tree Height is now " << bTree.GetHeight() << endl;
	}
	double  bTreeTime = ((double)(clock() - (double)start)) / CLOCKS_PER_SEC;

	cout << "BST Work[" << bstTree.GetApproxWorkDone() << "] - Time[" << BSTtime << "] - U-Nodes[" << bstTree.GetUnique() << "] - NU-Nodes[" << bstTree.GetNonUnique() << "]" << endl;
	cout << "AVL Work[" << avlTree.GetApproxWorkDone() << "] - Time[" << AVLtime << "] - U-Nodes[" << avlTree.GetUnique() << "] - NU-Nodes[" << avlTree.GetNonUnique() << "]" << endl;
	cout << "BTree Work[" << bTree.GetApproxWorkDone() << "] - Time[" << bTreeTime << "] - U-Nodes[" << bTree.GetUnique() << "] - NU-Nodes[" << bTree.GetNonUnique() << "]" << endl;
}

void RunLoopInsertOnBTree()
{
	BTree bTree = BTree("TestDisk.txt");

	char chari[10];
	for (int i = 0; i < 10; i++) chari[i] = '\0';

	clock_t start = clock();
	start = clock();
	for (int i = 1001; i <= 9200; i++) // insert 8200 strings in BTree
	{
		sprintf(chari, "%4i", i);
		bTree.Insert(chari);
		cout << "Tree Height is now " << bTree.GetHeight() << endl;
	}
	double  bTreeTime = ((double)(clock() - (double)start)) / CLOCKS_PER_SEC;

	cout << "BTree Work[" << bTree.GetApproxWorkDone() << "] - Time[" << bTreeTime << "] - U-Nodes[" << bTree.GetUnique() << "] - NU-Nodes[" << bTree.GetNonUnique() << "]" << endl;
}

void RunInsertFromFile(const char* filePath, int treeNum)
{
	BST bstTree = BST();
	AVL avlTree = AVL();
	BTree bTree = BTree("TestDisk.txt");

	char c;
	char chari[50]; // assumes no word is longer than 49 characters
	memset(chari, 0, 50); // zero the word buffer
	int iPtr;
	bool IsDelimiter = false, WasDelimiter = false;

	clock_t start = clock();
	ifstream inFile;
	inFile.open(filePath, ios::binary);
	if (inFile.fail())
	{
		cout << "Unable to open input file" << endl;
		return;
	}
	iPtr = 0;
	inFile.get(c); // priming read
	while (!inFile.eof())
	{
		IsDelimiter = (c == ' ' || c == 10 || c == 13 || c == 9 || c == '.' || c == ',' || c == '!' || c == ';' || c == ':' || c == '(' || c == ')' || c == '?' || c == '-');
		if (IsDelimiter && !WasDelimiter) // if THIS character IS a delimiter, and the last character WASN’T, it’s the end of a word
		{
			WasDelimiter = true;

			switch (treeNum)
			{
			case 2:
				bstTree.Insert(chari);
				break;
			case 3:
				avlTree.Insert(chari);
				break;
			case 4:
				bTree.Insert(chari);
				break;
			}

			memset(chari, 0, 50); // zero the word buffer
			iPtr = 0;
		}
		else if (!IsDelimiter) // if this isn’t a delimiter, keep going
		{
			chari[iPtr] = c;
			iPtr++;
		}
		else if (IsDelimiter && WasDelimiter)
		{
			// Do nothing -- two consecutive delimiters.
		}
		WasDelimiter = IsDelimiter; // for the NEXT iteration
		inFile.get(c);
	}
	inFile.close();
	double timeSpent = ((double)(clock() - (double)start)) / CLOCKS_PER_SEC;

	switch (treeNum)
	{
	case 1:
		cout << "Time[" << timeSpent << "]" << endl;
		break;
	case 2:
		cout << "BST Work[" << bstTree.GetApproxWorkDone() << "] - Time[" << timeSpent << "] - U-Nodes[" << bstTree.GetUnique() << "] - NU-Nodes[" << bstTree.GetNonUnique() << "]" << endl;
		break;
	case 3:
		cout << "AVL Work[" << avlTree.GetApproxWorkDone() << "] - Time[" << timeSpent << "] - U-Nodes[" << avlTree.GetUnique() << "] - NU-Nodes[" << avlTree.GetNonUnique() << "]" << endl;
		break;
	case 4:
		cout << "BTree Work[" << bTree.GetApproxWorkDone() << "] - Time[" << timeSpent << "] - U-Nodes[" << bTree.GetUnique() << "] - NU-Nodes[" << bTree.GetNonUnique() << "]" << endl;
		break;
	}
}

void RunBTreeInsertFromFile(const char* filePath)
{
	BTree bTree = BTree("TestDisk.txt");

	char c;
	char chari[32]; // assumes no word is longer than 32 characters
	memset(chari, 0, 32); // zero the word buffer
	int iPtr;
	bool IsDelimiter = false, WasDelimiter = false;

	clock_t start = clock();
	ifstream inFile;
	inFile.open(filePath, ios::binary);
	if (inFile.fail())
	{
		cout << "Unable to open input file" << endl;
		return;
	}
	iPtr = 0;
	inFile.get(c); // priming read
	while (!inFile.eof())
	{
		IsDelimiter = (c == ' ' || c == 10 || c == 13 || c == 9 || c == '.' || c == ',' || c == '!' || c == ';' || c == ':' || c == '(' || c == ')' || c == '?' || c == '-');
		if (IsDelimiter && !WasDelimiter) // if THIS character IS a delimiter, and the last character WASN’T, it’s the end of a word
		{
			WasDelimiter = true;
			bTree.Insert(chari);
			memset(chari, 0, 32); // zero the word buffer
			iPtr = 0;
		}
		else if (!IsDelimiter) // if this isn’t a delimiter, keep going
		{
			chari[iPtr] = c;
			iPtr++;
		}
		else if (IsDelimiter && WasDelimiter)
		{
			// Do nothing -- two consecutive delimiters.
		}
		WasDelimiter = IsDelimiter; // for the NEXT iteration
		inFile.get(c);
	}
	inFile.close();
	double timeSpent = ((double)(clock() - (double)start)) / CLOCKS_PER_SEC;
	cout << "BTree Work[" << bTree.GetApproxWorkDone() << "] - Time[" << timeSpent << "] - U-Nodes[" << bTree.GetUnique() << "] - NU-Nodes[" << bTree.GetNonUnique() << "]" << endl;
}

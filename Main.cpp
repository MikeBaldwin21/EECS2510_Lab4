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
#include "AVL.h"
#include "BTree.h"

using namespace std;

int RunCustomCommands();
void RunListing1();
void RunListing2();

int main(int argc, char* argv[])
{
	START:
	char initialCmd[50];
	cout << "Please enter in '1' for custom commands, '2' for Listing1(for(8200)), or '3' for Listing2(shakespeare.txt)" << endl;
	cin >> initialCmd;

	if(strcmp(initialCmd, "1") == 0)
		RunCustomCommands();
	if(strcmp(initialCmd, "2") == 0)
		RunListing1();
	if(strcmp(initialCmd, "3") == 0)
		RunListing2();
	else
		goto START;
	return 0;
}

int RunCustomCommands()
{
	AVL avlTree = AVL();
	BTree bTree = BTree();

	while (true)
	{
		char str1[50];
		std::cin >> str1;

		if (std::strlen(str1) > 0)
		{
			if (std::strcmp(str1, "insert") == 0)
			{
				char str2[50];
				std::cin >> str2;
				bst.Insert(str2);
				avl.Insert(str2);
				rbt.Insert(str2);
				skipList.Insert(str2);
			}
			else if (std::strcmp(str1, "search") == 0)
			{
				char str2[50];
				std::cin >> str2;
				bst.Search(str2);
				avl.Search(str2);
				rbt.Search(str2);
				skipList.Search(str2);
			}
			else if (std::strcmp(str1, "exit") == 0)
			{
				return 0;
			}
		}
	}
}

void RunListing1()
{
	char c;
	RBT RBT_T; // instantiate each of the trees
	AVL AVL_T; //
	BST BST_T; //
	SkipList SL; // and the skip list
	char chari[10];
	for (int i = 0; i < 10; i++) chari[i] = '\0';

	clock_t start = clock();
	for (int i = 1001; i <= 9200; i++) // insert 8200 strings in RBT
	{
		std::sprintf(chari, "%4i", i);
		RBT_T.Insert(chari);
		std::cout << "Tree Height is now " << RBT_T.GetHeight() << std::endl;
	}
	double  RBTtime = ((double)(clock() - start)) / CLOCKS_PER_SEC;
	start = clock();
	for (int i = 1001; i <= 9200; i++) // insert 8200 strings in AVL Tree
	{
		std::sprintf(chari, "%4i", i);
		AVL_T.Insert(chari);
		std::cout << "Tree Height is now " << AVL_T.GetHeight() << std::endl;
	}
	double AVLtime = ((double)(clock() - start)) / CLOCKS_PER_SEC;
	start = clock();
	for (int i = 1001; i <= 9200; i++) // insert 8200 strings in BST
	{
		std::sprintf(chari, "%4i", i);
		BST_T.Insert(chari);
		std::cout << "Tree Height is now " << BST_T.GetHeight() << std::endl;
	}
	double BSTtime = ((double)(clock() - start)) / CLOCKS_PER_SEC;
	start = clock();
	for (int i = 1001; i <= 9200; i++) // insert 8200 strings in SkipLists
	{
		std::sprintf(chari, "%4i", i);
		SL.Insert(chari);
		std::cout << "List Height is now " << SL.GetHeight() << std::endl;
	}
	double SLtime = ((double)(clock() - start)) / CLOCKS_PER_SEC;

	std::cout << "RBT Work[" << RBT_T.GetApproxWorkDone() << "] - Time[" << RBTtime << "] - U-Nodes[" << RBT_T.GetUnique() << "] - NU-Nodes[" << RBT_T.GetNonUnique() << "]" << std::endl;
	std::cout << "AVL Work[" << AVL_T.GetApproxWorkDone() << "] - Time[" << AVLtime << "] - U-Nodes[" << AVL_T.GetUnique() << "] - NU-Nodes[" << AVL_T.GetNonUnique() << "]" << std::endl;
	std::cout << "BST Work[" << BST_T.GetApproxWorkDone() << "] - Time[" << BSTtime << "] - U-Nodes[" << BST_T.GetUnique() << "] - NU-Nodes[" << BST_T.GetNonUnique() << "]" << std::endl;
	std::cout << "SL  Work[" << SL.GetApproxWorkDone() << "] - Time[" << SLtime << "] - Nodes[" << SL.GetNodeCount() << "]" << std::endl;

	std::cout << "\n\nPress ENTER to exit\n"; std::cin.get(c);
}

void RunListing2()
{
	char c;
	RBT RBT_T;
	AVL AVL_T;
	BST BST_T;
	SkipList SL; // and the skip list
	char chari[50]; // assumes no word is longer than 49 characters
	memset(chari, 0, 50); // zero the word buffer
	int iPtr;
	bool IsDelimiter = false, WasDelimiter = false;

	clock_t start = clock();
	std::ifstream inFile;
	inFile.open("ALL.txt", std::ios::binary);
	if (inFile.fail())
	{
		std::cout << "Unable to open input file\n\n"
			<< "Program Exiting\n\nPress ENTER to exit\n";
		std::cin.get(c);
		exit(1);
	}
	iPtr = 0;
	inFile.get(c); // priming read
	while (!inFile.eof())
	{
		IsDelimiter = (c == ' ' || c == 10 || c == 13 || c == 9 || c == '.' || c == ',' ||
			c == '!' || c == ';' || c == ':' || c == '(' || c == ')' || c == '?' ||
			c == '-');
		if (IsDelimiter && !WasDelimiter) // if THIS character IS a delimiter, and the
		// last character WASN’T, it’s the end of a word
		{
			WasDelimiter = true;
			//RBT_T.Insert(chari); // insert this word in the RBT
			//AVL_T.Insert(chari); // insert it in the AVL Tree
			//BST_T.Insert(chari); // insert it in the BST
			SL.Insert(chari); // insert it in the skip list
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
	double timeSpent = ((double)(clock() - start)) / CLOCKS_PER_SEC;
	//std::cout  << "Time[" << timeSpent << "]" << std::endl;
	//std::cout << "RBT Work[" << RBT_T.GetApproxWorkDone() << "] - Time[" << timeSpent << "] - U-Nodes[" << RBT_T.GetUnique() << "] - NU-Nodes[" << RBT_T.GetNonUnique() << "]" << std::endl;
	//std::cout << "AVL Work[" << AVL_T.GetApproxWorkDone() << "] - Time[" << timeSpent << "] - U-Nodes[" << AVL_T.GetUnique() << "] - NU-Nodes[" << AVL_T.GetNonUnique() << "]" << std::endl;
	//std::cout << "BST Work[" << BST_T.GetApproxWorkDone() << "] - Time[" << timeSpent << "] - U-Nodes[" << BST_T.GetUnique() << "] - NU-Nodes[" << BST_T.GetNonUnique() << "]" << std::endl;
	std::cout << "SL  Work[" << SL.GetApproxWorkDone() << "] - Time[" << timeSpent << "] - Nodes[" << SL.GetNodeCount() << "]" << std::endl;
	std::cout << "\n\nPress ENTER to exit\n"; std::cin.get(c);
}
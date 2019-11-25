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
#include "BST.h"
#include "AVL.h"
#include "BTree.h"
#include "Constants.h"

using namespace std;

void RunInsertFromFile(const char* filePath, int treeNum);
int GetFileExtensionSize(string filePath);

int main(int argc, char* argv[])
{
	// I would only comment this out if you have about 5 hours to spare!!!!

	/*string filePaths[] = { "Docs/A Bee Movie Script.txt", "Docs/ALL.txt", "Docs/Anna Karenina - Tolstoy.txt", "Docs/Bleak House - Dickens.txt", "Docs/David Copperfield - Dickens.txt", "Docs/Don Quixote - Cervantes.txt", "Docs/Green Eggs and Ham.txt", "Docs/Hamlet.txt", "Docs/Hamlet-Scene-1.txt", "Docs/King James Bible.txt", "Docs/Les Miserables - Hugo.txt", "Docs/Middlemarch - Eliot.txt", "Docs/Moby Dick - Melville.txt", "Docs/Shakespeare.txt", "Docs/TEST.txt", "Docs/The Brothers Karamazof - Dostoyevsky.txt", "Docs/The Hunchback of Notre Dame - Hugo.txt", "Docs/The Three Musketeers - Dumas.txt", "Docs/To Kill a Mockingbird - Harper Lee.txt", "Docs/War and Peace.txt" };*/



	string filePaths[] = { "Docs/Green Eggs and Ham.txt", "Docs/Hamlet.txt", "Docs/Hamlet-Scene-1.txt", "Docs/TEST.txt", "Docs/To Kill a Mockingbird - Harper Lee.txt" };

	for (int j = 0; j < 5; j++)
	{
		for (int i = 0; i < 5; i++)
		RunInsertFromFile(filePaths[j].c_str(), i);
	}
	return 0;
}

void RunInsertFromFile(const char* filePath, int treeNum)
{
	// The last sentence on the Lab4.pdf says we can use strings, deduct no points please!
	string inputFilePath = filePath;
	string outputFilePath;
	int extSize = GetFileExtensionSize(filePath);
	if (extSize > 0)
		outputFilePath = inputFilePath.substr(0, inputFilePath.length() - extSize) + ".btree";
	else
		outputFilePath = inputFilePath + ".btree";

	BST* bstTree = nullptr;
	AVL* avlTree = nullptr;
	BTree* bTree = nullptr;

	switch (treeNum)
	{
	case 1:
		cout << endl << "Input FilePath: " << inputFilePath << endl;
		cout << "Output FilePath: " << outputFilePath << endl;
		break;
	case 2:
		bstTree = new BST();
		break;
	case 3:
		avlTree = new AVL();
		break;
	case 4:
		bTree = new BTree(outputFilePath.c_str());
		break;
	}

	char c;
	char chari[MAX_DATA_LENGTH]; // assumes no word is longer than 49 characters
	memset(chari, 0, sizeof(chari)); // zero the word buffer
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
				bstTree->Insert(chari);
				break;
			case 3:
				avlTree->Insert(chari);
				break;
			case 4:
				bTree->Insert(chari);
				break;
			}

			memset(chari, 0, sizeof(chari)); // zero the word buffer
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
		/*case 1:
			cout << "Time[" << timeSpent << "]" << endl;
			break;*/
	case 2:
		cout << "BST Time[" << timeSpent << "] - BST Reads[" << bstTree->GetReadCount() << "] - BST Writes[" << bstTree->GetWriteCount() << "] - Total R/W[" << bstTree->GetReadCount() + bstTree->GetWriteCount() << "]" << endl;
		delete bstTree;
		break;
	case 3:
		cout << "AVL Time[" << timeSpent << "] - AVL Reads[" << avlTree->GetReadCount() << "] - AVL Writes[" << avlTree->GetWriteCount() << "] - Total R/W[" << avlTree->GetReadCount() + avlTree->GetWriteCount() << "]" << endl;
		delete avlTree;
		break;
	case 4:
		cout << "BTree Time[" << timeSpent << "] - BTree Reads[" << bTree->GetReadCount() << "] - BTree Writes[" << bTree->GetWriteCount() << "] - Total R/W[" << bTree->GetReadCount() + bTree->GetWriteCount() << "]" << endl;
		delete bTree;
		break;
	}
}

int GetFileExtensionSize(string filePath)
{
	/*
	 * Helper function to find the extension of a file (it it exists), from a file path
	 */

	size_t lastDot = filePath.find_last_of('.'); // Find the last dot character
	if (lastDot != string::npos) // If found, get the size of the extension and return that, else return 0
		return filePath.length() - lastDot;
	return 0;
}
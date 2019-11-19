/*
 * Tree.h
 * Name: Mike Baldwin
 * Course: EECS 2510 Non-Linear Data Structures - Fall 2019
 * Date: 11/24/2019
 * Description: Declaration of the abstract tree class
*/

#pragma once

class Tree
{
public:
	virtual void Insert(const char* x) = 0;
	virtual void Search(const char* cArray) = 0;
	virtual int GetHeight() = 0;
	virtual int GetApproxWorkDone() = 0;
	virtual int GetUnique() = 0;
	virtual int GetNonUnique() = 0;
};
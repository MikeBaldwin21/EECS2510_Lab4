/*
 * AVL.cpp
 * Name: Mike Baldwin
 * Course: EECS 2510 Non-Linear Data Structures - Fall 2019
 * Date: 11/24/2019
 * Description: Implementation of the AVL tree
*/

#include <string.h>
#include <iostream>
#include <assert.h>
#include "AVL.h"

AVL::AVL() : root{ nullptr }, keyComparisonCount{ 0 }, bfChangeCount{ 0 }
{
}

void AVL::Insert(const char* x)
{
	/*
	 *	Inserts a string into the tree. If the string already exists, then it will increment it's count.
	 *	Parameter:
	 *		<str> This is the string to insert into the tree
	*/

	int d; // displacement; Used to adjust BFs

	//==============================================================
	// EMPTY TREE INSERT CODE
	//==============================================================

	// If the root is null, then we create a new node and stop the insert here
	if (root == nullptr)
	{
		Node* y = new Node(x, 1, nullptr, nullptr, 0);
		root = y;
		return;
	}

	//==============================================================
	// SEARCH AND INSERT CODE (SAME OLD BST INSERT)
	//==============================================================

	/*
	 * Locate insertion point for the new node.
	 * The scanner scans through the tree until it falls off bottom
	 * Q is P’s parent (Q lags one step behind P) (y previously)
	 * The new Node Y will be added as either the Left or Right child of Q
	 * A is the last parent above Y with a BF of ±1 (before the insert)
	 * F is A’s parent (F lags one step behind A)
	 */

	Node* p = root; // The current node we are using to scan through the tree
	Node* q = nullptr; // The parent of the current node (lags behind current node)
	Node* a = root; // The last non-zero balance factor node
	Node* f = nullptr; // The parent of the last non-zero balance factor node

	while (p != nullptr) // search tree for insertion point. We can't use the search function as we need to track the last non-zero bf node (and it's parent)
	{
		keyComparisonCount++;
		if (strcmp(x, p->data) == 0) // Found a node that matches, just increment count and return (no balancing needed)
		{
			p->count++;
			return;
		}
		if (p->bf != 0) // If the current node has a non-zero balance factor, remember it (and its parent)
		{
			a = p;
			f = q;
		}

		// Advance the current node to it's next child (based upon the BT property). Also bring it's parent up to its old spot.
		q = p;
		p = strcmp(x, p->data) < 0 ? p->left : p->right;
		keyComparisonCount++;
	}

	// If we reach here, then current node is NULL. Which is fine, we need to make a new node anyways...
	Node* y = new Node(x, 1, nullptr, nullptr, 0);

	// Based upon currentNodeParent, we will either place this new node as its left or right child
	if (strcmp(x, q->data) < 0)
		q->left = y;
	else
		q->right = y;
	keyComparisonCount++;

	//==============================================================
	// TREE BALANCING
	//==============================================================

	/*
	 * Adjust BFs from lastNZBFNode to CurrentNodeParent.
	 * lastNZBFNode was the LAST ancestor with a BF of ± 1, (or is still the root, because we never FOUND a BF of ± 1).
	 * So ALL nodes BETWEEN lastNZBFNode and CurrentNodeParent must have a BF of 0, and will, therefore, BECOME ± 1.
	 *
	 * If newNode is inserted in the LEFT subtree of lastNZBFNode, then displacement = +1
	 * If displacement == -1 means we inserted newNode in the RIGHT subtree of lastNZBFNode.
	 */

	Node* b; // This the child of the last non-zero branch factor node we encountered

	keyComparisonCount++;
	// Which way is the displacement
	if (strcmp(x, a->data) > 0) // Is the new node on the right side of the lastNZBFNode
	{
		b = a->right; // Set lastNZBFNodeChild to lastNZBFNodes right child
		p = a->right; // Set currentNode to lastNZBFNodes right child
		d = -1; // Displacement is off by -1 now
	}
	else // Is the new node on the left side of the lastNZBFNode
	{
		b = a->left; // Set lastNZBFNodeChild to lastNZBFNodes left child
		p = a->left; // Set currentNode to lastNZBFNodes left child
		d = 1; // Displacement is off by +1 now
	}

	// currentNode is now one node below lastNZBFNode. Adjust from here to the insertion point. Don’t do anything to newNode.
	while (p != y /*&& currentNode != nullptr*/)
	{
		assert(p != nullptr);

		keyComparisonCount++;
		// If newNode is on the right side of currentNode (which means the newnode is on the LEFT side of the currentNode)
		if (strcmp(x, p->data) > 0)
		{
			// adjust BF and move forward
			p->bf = -1;
			p = p->right;
			bfChangeCount++;
		}
		else // If newNode is on the left side of currentNode (which means the newnode is on the RIGHT side of the currentNode)
		{
			// adjust BF and move forward
			p->bf = 1;
			p = p->left;
			bfChangeCount++;
		}
	}

	// If the tree was completely balanced then all we do is set the new balance factor and return
	if (a->bf == 0)
	{
		bfChangeCount++;
		a->bf = d; // Set the bf to ± 1 (the displacement tells us which direction)
		return;
	}
	// If the tree already had a slight imbalance the OTHER way, then did the insertion throw the tree into complete balance. If so, set the BF to zero and return
	if (a->bf == -d)
	{
		bfChangeCount++;
		a->bf = 0; // Set bf to zero and return
		return;
	}

	//If we took neither of the two returns above then the tree was acceptably imbalanced, and is now UNACCEPTABLY IMBALANCED.
	if (d == 1)
	{
		assert(b != nullptr);

		if (b->bf == 1) // LL rotation
		{
			// Change the child pointers of lastNZBFNode and lastNZBFNodeChild to reflect the new post-rotation structure
			a->left = b->right;
			b->right = a;
			a->bf = b->bf = 0;
			bfChangeCount += 2;
		}
		else // LR rotation
		{
			/*
			 * LR rotation (4 LOC)
			 * 3 CASES
			 * Change the child pointers of lastNZBFNode, lastNZBFNodeChild, and lastNZBFNodeGrandChild to reflect the new post-rotation structure
			 */

			Node* c = b->right;
			Node* cl = c->left;
			Node* cr = c->right;

			/*
			 * Set BF's of A & B, based upon the BF of C
			 * 3 sub-cases
			 * 3 LOC
			 */
			switch (c->bf)
			{
			case 0: // CASE (a)
				c->right = a;
				c->left = b;
				a->left = b->right = nullptr;
				a->bf = b->bf = c->bf = 0;
				bfChangeCount += 3;
				break;
			case 1: // CASE (b)
				c->right = a;
				c->left = b;
				b->right = cl;
				a->left = cr;
				b->bf = 0;
				c->bf = 0;
				a->bf = -1;
				bfChangeCount += 3;
				break;
			case -1: // CASE (c)
				c->right = a;
				c->left = b;
				b->right = cl;
				a->left = cr;
				b->bf = 1;
				c->bf = 0;
				a->bf = 0;
				bfChangeCount += 3;
				break;
			}

			c->bf = 0;
			b = c;
		}
	}
	else // displacement = -1
	{
		//Symmetric to above

		assert(b != nullptr);

		if (b->bf == -1) // RR rotation
		{
			a->right = b->left;
			b->left = a;
			a->bf = b->bf = 0;
			bfChangeCount += 2;
		}
		else
		{
			/*
			 * RL rotation (4 LOC)
			 * 3 CASES
			 * Change the child pointers of lastNZBFNode, lastNZBFNodeChild, and lastNZBFNodeGrandChild to reflect the new post-rotation structure
			 */

			Node* c = b->left;
			Node* cl = c->left;
			Node* cr = c->right;

			/*
			 * Set BF's of A & B, based upon the BF of C
			 * 3 sub-cases
			 * 3 LOC
			 */
			switch (c->bf)
			{
			case 0: // CASE (a)
				c->left = a;
				c->right = b;
				a->right = b->left = nullptr;
				a->bf = b->bf = c->bf = 0;
				bfChangeCount += 3;
				break;
			case 1: // CASE (b)
				c->left = a;
				c->right = b;
				b->left = cr;
				a->right = cl;
				a->bf = 0;
				c->bf = 0;
				b->bf = -1;
				bfChangeCount += 3;
				break;
			case -1: // CASE (c)
				c->left = a;
				c->right = b;
				b->left = cr;
				a->right = cl;
				a->bf = 1;
				c->bf = 0;
				b->bf = 0;
				bfChangeCount += 3;
				break;
			}

			c->bf = 0;
			b = c;
		}
	}

	//==============================================================
	// FINISH UP
	//==============================================================

	/*
	 * Regardless, the subtree rooted at lastNZBFNodeChild has been rebalanced and needs to be the new child of lastNZBFNodeParent
	 * The original subtree of lastNZBFNodeParent had a root of lastNZBFNode
	 */

	 // Did we re-balance the whole tree's root?
	if (f == nullptr)
	{
		root = b;
		return;
	}

	/*
	 * The root of what we rebalanced WAS lastNZBFNode, now it’s lastNZBFNodeChild.
	 * If the subtree we rebalanced was LEFT of lastNZBFNodeParent, then lastNZBFNodeChild needs to be left of lastNZBFNodeParent.
	 * If lastNZBFNode was RIGHT of lastNZBFNodeParent, then lastNZBFNodeChild now needs to be right of lastNZBFNodeParent.
	*/

	if (a == f->left)
	{
		f->left = b;
		return;
	}
	if (a == f->right)
	{
		f->right = b;
		return;
	}
	std::cout << "We should never be here" << std::endl;
}

void AVL::Search(const char* cArray)
{
	/*
	 * Searches in the entire tree for a string and prints to console if found (or if not found)
	 * Parameter<str> The string to search for
	*/

	Node* node = Find(cArray);
	if (node != nullptr)
		std::cout << node->data << ' ' << node->count << std::endl;
	else
		std::cout << cArray << " 0" << std::endl;
}

int AVL::GetHeight()
{
	return ComputeHeight(root);
}

int AVL::GetApproxWorkDone()
{
	return keyComparisonCount + bfChangeCount;
}

int AVL::GetUnique()
{
	return TraverseUnique(root);
}

int AVL::GetNonUnique()
{
	return TraverseNonUnique(root);
}

AVL::Node* AVL::Find(const char* cArray)
{
	/*
	 * Used for searching the tree when you want to get a node pointer back as opposed to just a string.
	 * Parameter<str> The string to search for
	 * Returns the node that contains the supplied string, or NULL if not found
	*/

	Node* current = root;
	while (current != nullptr) //'Current' will be NULL if 'root == NULL', which is intended!
	{
		keyComparisonCount++;
		if (std::strcmp(cArray, current->data) == 0)
			return current;
		keyComparisonCount++;
		if (std::strcmp(cArray, current->data) < 0)
			current = current->left;
		else
			current = current->right;
	}
	return current;
}

int AVL::TraverseNonUnique(Node* node)
{
	/*
	 * Traverses the tree (or subtree), calls Traversal on it's children, and returns the count of nodes.
	 * Parameter<node> The node to start the traversal from (in this subtree)
	*/

	if (node == nullptr)
		return 0;

	int leftSideUnique = TraverseNonUnique(node->left);
	int rightSideUnique = TraverseNonUnique(node->right);
	return leftSideUnique + rightSideUnique + 1;
}

int AVL::TraverseUnique(Node* node)
{
	/*
	 * Traverses the tree (or subtree), calls Traversal on it's children, and returns the count of nodes that have multiple items.
	 * Parameter<node> The node to start the traversal from (in this subtree)
	*/

	if (node == nullptr)
		return 0;

	int leftSideUnique = TraverseUnique(node->left);
	int rightSideUnique = TraverseUnique(node->right);
	return leftSideUnique + rightSideUnique + ((node->count > 1) ? 1 : 0);
}

int AVL::ComputeHeight(Node* node)
{
	/*
	 * Trivial recursive traversal function. It calculates the height.
	*/

	if (node == nullptr)
		return 0;
	else
	{
		int leftHeight = ComputeHeight(node->left);
		int rightHeight = ComputeHeight(node->right);

		if (leftHeight > rightHeight)
			return (leftHeight + 1);
		else
			return rightHeight + 1;
	}
}

AVL::Node::Node(const char* name, const int count, Node* left, Node* right, int balanceFactor)
{
	std::strcpy(this->data, name);
	this->count = count;
	this->left = left;
	this->right = right;
	this->bf = balanceFactor;
}

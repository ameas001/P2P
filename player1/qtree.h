// Name: Chris Wong
// Quarter, Year: spring 2013
// Lab: 2
//
// This file is to be modified by the student.
//	quadtree.h
////////////////////////////////////////////////////////////
#ifndef __QUADTREE_H__
#define __QUADTREE_H__

#include <vector>
#include "object.h"
#include <iostream>


using namespace std;

enum QuadTreeType { QUAD_TREE_PARENT, QUAD_TREE_LEAF };

class QuadTreeNode
{
public:
	QuadTreeNode(const Rect2D & space);

	void insert(const Line2D & value, int currentDepth, int depthLimit, int listLimit);
	void query(const Point2D & mbr, std::vector<Line2D> & ret);
	void dealloc();
	void render() const;
private:
	//Shared between Parent and Leaf Nodes
	QuadTreeType nodeType;
	Rect2D space;
	
	//Information for Leaf Nodes
	std::vector<Line2D> objects;

	//Information for Parent Nodes
	QuadTreeNode* topLeft;
	QuadTreeNode* topRight;
	QuadTreeNode* bottomLeft;
	QuadTreeNode* bottomRight;

	//Useful Helper Functions
	QuadTreeNode* findQuadrant(const Point2D & p);
	int findQuadrantnum(const Point2D & p) const;
	std::vector<Line2D> splitLine(const Line2D & value) const;
};

//A Wrapper for the QuadTreeNode class
class QuadTree
{
public:
	QuadTree(const Rect2D & space, int dLim, int listLim);
	~QuadTree();

	void insert(const Line2D & value);
	std::vector<Line2D> query(const Point2D & p);
	std::vector<Line2D> query(const Point2D & p, const Circle2D c);
	
	void render() const;
private:
	QuadTreeNode* root;
	
	//These are additional variables to simplify the QuadTree
	//
	//A LEAF node cannot become a PARENT node if its depth in the recursion
	//is greater than or equal to depthLimit.
	//
	//The listLimit variable is a relaxed limit to notify when a LEAF node
	//should split. This condition is not met when the LEAF node is already
	//at the lowest depth in the tree.
	int depthLimit;
	int listLimit;
};

#endif

// Name: 
// Quarter, Year: 
// Lab:
//
// This file is to be modified by the student.
// quadtree.cpp
////////////////////////////////////////////////////////////
#include "quadtree.h"

using namespace std;

QuadTreeNode::QuadTreeNode(const Rect2D & space)
	: nodeType(QUAD_TREE_LEAF), space(space), objects(), topLeft(NULL), topRight(NULL), bottomLeft(NULL), bottomRight(NULL)
{}

int QuadTreeNode::getTheQuad(const Point2D & p) const 
{
	double tx = space.center().x;
	double ty = space.center().y;
	if(p.x <= tx && p.y >= ty) return 2;
	else if(p.x > tx && p.y >= ty) return 1;
	else if(p.x <= tx && p.y < ty) return 3;
	else if(p.x > tx && p.y < ty) return 4;
	else return 0;
}

void QuadTreeNode::insert(const Line2D & value, int currentDepth, int depthLimit, int listLimit)
{
	if (nodeType == QUAD_TREE_PARENT)
	{
		QuadTreeNode* t1 = findQuadrant(value.p1);
		QuadTreeNode* t2 = findQuadrant(value.p2);
		if(t1 == t2) // no split
		  t1->insert(value,currentDepth,depthLimit,listLimit);
		else // needs to be split
		{
		  std::vector<Line2D> lines = splitLine(value);
			for(int i = 0; i < lines.size(); i++)
				insert(lines[i],currentDepth,depthLimit,listLimit);
		}
	}
	else //(nodeType == QUAD_TREE_LEAF)
	{
	  if(objects.size() < listLimit)
			objects.push_back(value);
		else
		{
			nodeType = QUAD_TREE_PARENT;
			double w = space.width / 2;
			double h = space.height / 2;
			
			Rect2D r = Rect2D(Point2D(space.p.x, space.p.y), w, h);
			topLeft = new QuadTreeNode(r);			
			
			r = Rect2D(Point2D(w + space.p.x, space.p.y), w, h);
			topRight = new QuadTreeNode(r);
			
			r = Rect2D(Point2D(space.p.x, h + space.p.y), w, h);
			bottomLeft = new QuadTreeNode(r);
			
			r = Rect2D(Point2D(w + space.p.x, h + space.p.y), w, h);
			bottomRight = new QuadTreeNode(r);
			insert(objects[0], currentDepth, depthLimit, listLimit);
			insert(value, currentDepth, depthLimit, listLimit);
		}
	}
}

void QuadTreeNode::query(const Point2D & p, std::vector<Line2D> & ret)
{
	if (nodeType == QUAD_TREE_PARENT){
		QuadTreeNode* t = findQuadrant(p);
		t->query(p,ret);
	}
	else if (nodeType == QUAD_TREE_LEAF){
		space.render(GRAY,true);
		ret = objects;
	}
}

vector<Line2D> QuadTree::query(const Point2D & p,const Circle2D c)
{
	vector<Line2D> ret;
	vector<Point2D> points;
	vector<Line2D> ret_temp;
	
	
	points.push_back(Point2D(c.p.x + c.radius,c.p.y + c.radius ));
	points.push_back(Point2D(c.p.x - c.radius,c.p.y + c.radius ));
	points.push_back(Point2D(c.p.x + c.radius,c.p.y - c.radius ));
	points.push_back(Point2D(c.p.x - c.radius,c.p.y - c.radius ));
	
	for(int i = 0 ; i < points.size(); i++)
	{
		ret_temp = query(points[i]);
		for(int j = 0; j < ret_temp.size(); j++)
			ret.push_back(ret_temp[j]);
	}
	return ret;
	
}

QuadTreeNode* QuadTreeNode::findQuadrant(const Point2D & p)
{
	double tx = space.center().x;
	double ty = space.center().y;
	if(p.x <= tx && p.y >= ty) return bottomLeft;
	else if(p.x > tx && p.y >= ty) return bottomRight;
	else if(p.x <= tx && p.y < ty) return topLeft;
	else if(p.x > tx && p.y < ty) return topRight;
	else return NULL;
}

std::vector<Line2D> QuadTreeNode::splitLine(const Line2D & value) const
{
	double s = .01;
	
	//Find the quadrants
	int t1 = getTheQuad(value.p1);
	int t2 = getTheQuad(value.p2);
	std::vector<Line2D> lines;
	
	//Horizontal line
	if(((t1 == 1 || t2 == 1) || (t1 == 3 || t2 == 3 ))  && 
	   ((t1 == 2 || t2 == 2) || (t1 == 4 || t2 == 4)) )
	{
		double l,r;
		if(t1 ==1 || t1 == 3)
		{ 
			l = value.p1.x; 
			r = value.p2.x;
		}
		else
		{ 
			l = value.p2.x; 
			r = value.p1.x; 
		}
		lines.push_back(Line2D(Point2D(l, value.p1.y), Point2D(space.center().x - s, value.p1.y)) );
		lines.push_back(Line2D(Point2D(space.center().x + s, value.p1.y), Point2D(r, value.p1.y)) );
	}
	//vertical line
	else
	{
		double t, b;
		if(t1 == 1 || t1 == 2) 
		{
			t = value.p1.y; 
			b = value.p2.y;
		}
		else 
		{
			t = value.p2.y; 
			b = value.p1.y;
		}
		lines.push_back(Line2D(Point2D(value.p1.x, t), Point2D(value.p1.x, space.center().y + s)));
		lines.push_back(Line2D(Point2D(value.p1.x, b), Point2D(value.p1.x, space.center().y - s)));
	}
	return lines;
}

//Recursively deletes the QuadTree
void QuadTreeNode::dealloc()
{
	if (topLeft)
	{
		topLeft->dealloc();
		delete topLeft;
	}
	if (topRight)
	{
		topRight->dealloc();
		delete topRight;
	}
	if (bottomLeft)
	{
		bottomLeft->dealloc();
		delete bottomLeft;
	}
	if (bottomRight)
	{
		bottomRight->dealloc();
		delete bottomRight;
	}
}

void QuadTreeNode::render() const
{
	if (nodeType == QUAD_TREE_PARENT)
	{
		topRight->space.render(BLACK);
		topLeft->space.render(BLACK);
		bottomLeft->space.render(BLACK);
		bottomRight->space.render(BLACK);
		
		topLeft->render();
		topRight->render();
		bottomLeft->render();
		bottomRight->render();
	}
	else //if(nodeType == QUAD_TREE_LEAF)
		for(int i = 0; i < objects.size(); i++)	objects[i].render(ORANGE);
}

QuadTree::QuadTree(const Rect2D & space, int dlim, int llim)
	: root(new QuadTreeNode(space)), depthLimit(dlim), listLimit(llim)
{}

QuadTree::~QuadTree()
{
	root->dealloc();
}

void QuadTree::insert(const Line2D & value)
{
	root->insert(value, 1, depthLimit, listLimit);
}

std::vector<Line2D> QuadTree::query(const Point2D & p) const
{
	std::vector<Line2D> ret;
	root->query(p, ret);
	return ret;
}

void QuadTree::render() const
{
	root->render();
}

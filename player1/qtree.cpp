// Name:
// Quarter, Year:
// Lab:
//
// This file is to be modified by the student.
// quadtree.cpp
////////////////////////////////////////////////////////////
#include "quadtree.h"
#include "math.h"

QuadTreeNode::QuadTreeNode(const Rect2D & space)
	: nodeType(QUAD_TREE_LEAF), space(space), objects(), topLeft(NULL), topRight(NULL), bottomLeft(NULL), bottomRight(NULL)
{}

//Find the quadrant helper function
QuadTreeNode* QuadTreeNode::findQuadrant(const Point2D & p){
	if(p.x <= space.center().x && p.y >= space.center().y) return bottomLeft;
	else if(p.x > space.center().x && p.y >= space.center().y) return bottomRight;
	else if(p.x <= space.center().x && p.y < space.center().y) return topLeft;
	else if(p.x > space.center().x && p.y < space.center().y) return topRight;
	else return NULL;
}

//Find the quadrant helper function
int QuadTreeNode::findQuadrantnum(const Point2D & p) const {
	if(p.x <= space.center().x && p.y >= space.center().y) return 2;
	else if(p.x > space.center().x && p.y >= space.center().y) return 1;
	else if(p.x <= space.center().x && p.y < space.center().y) return 3;
	else if(p.x > space.center().x && p.y < space.center().y) return 4;
	else return 0;
}

//split line helper function
std::vector<Line2D> QuadTreeNode::splitLine(const Line2D & value) const{
	
	double gap = .01;
	
	//Find the quadrants
	int p1quad = findQuadrantnum(value.p1);
	int p2quad = findQuadrantnum(value.p2);
	std::vector<Line2D> lines;
	
	//Horizontal line
	if(((p1quad == 1 || p2quad == 1) || (p1quad == 3 || p2quad == 3 ))  && 
	   ((p1quad == 2 || p2quad == 2) || (p1quad == 4 || p2quad == 4)) ){
		double leftpnt,rightpnt;
		if(p1quad ==1 || p1quad == 3){ leftpnt = value.p1.x; rightpnt = value.p2.x;}
		else{ leftpnt = value.p2.x; rightpnt = value.p1.x; }
		lines.push_back(Line2D(Point2D(leftpnt,value.p1.y),Point2D(space.center().x- gap,value.p1.y)) );
		lines.push_back(Line2D(Point2D(space.center().x+ gap,value.p1.y),Point2D(rightpnt,value.p1.y)) );
	}
	//vertical line
	else{
		double toppnt, botpnt;
		if(p1quad == 1 || p1quad == 2) {toppnt = value.p1.y; botpnt = value.p2.y;}
		else {toppnt = value.p2.y; botpnt = value.p1.y;}
		lines.push_back(Line2D(Point2D(value.p1.x,toppnt),Point2D(value.p1.x,space.center().y+ gap)));
		lines.push_back(Line2D(Point2D(value.p1.x,botpnt),Point2D(value.p1.x,space.center().y- gap)));
	}
	
	return lines;
}

void QuadTreeNode::insert(const Line2D & value, int currentDepth, int depthLimit, int listLimit)
{
	cout << "deciding on->"<< value.p1.x << "," << value.p1.y  << "::" ;
	
	if (nodeType == QUAD_TREE_PARENT){
		
		//Find the quadrants
		QuadTreeNode* p1quad = findQuadrant(value.p1);
		QuadTreeNode* p2quad = findQuadrant(value.p2);
		
		//dont need to line split
		if(p1quad == p2quad){
			cout << "dont need to split" << endl;
			p1quad->insert(value,currentDepth,depthLimit,listLimit);
		}
		
		//else needs to be split
		else{
			std::vector<Line2D> lines = splitLine(value);
			cout << "splitting" << endl;
			for(int i = 0; i < lines.size(); i++){
				insert(lines[i],currentDepth,depthLimit,listLimit);
			}
			
		}
	}
	else if (nodeType == QUAD_TREE_LEAF){
		
		//if not full
	    if(objects.size() != listLimit){ 
			objects.push_back(value);
			cout << "adding line to leaf" << ";" << nodeType << endl;
		}
		
		//else
		else{
			
			//make into parent
			cout << "making into parent" << endl;
			nodeType = QUAD_TREE_PARENT;
			double x1 = space.p.x;
			double y1 = space.p.y;
			topLeft = new QuadTreeNode( Rect2D(Point2D(x1,y1),space.width/2,space.height/2));
			topRight = new QuadTreeNode( Rect2D(Point2D(space.center().x,y1),space.width/2,space.height/2));
			bottomLeft = new QuadTreeNode(Rect2D(Point2D(x1,space.center().y),space.width/2,space.height/2));
			bottomRight = new QuadTreeNode(Rect2D(Point2D(space.center().x,space.center().y),space.width/2,space.height/2));
			
			//reinsert lines into leaf
			insert(objects[0],currentDepth,depthLimit,listLimit);
			
			//insert new value
			insert(value,currentDepth,depthLimit,listLimit);
		}
	}
}

void QuadTreeNode::query(const Point2D & p, std::vector<Line2D> & ret)
{
	Point2D pnew = p;
	
	if (nodeType == QUAD_TREE_PARENT){
		QuadTreeNode* p1quad = findQuadrant(pnew);
		p1quad->query(p,ret);
	
	}
	else if (nodeType == QUAD_TREE_LEAF){
		space.render(GREEN,true);
		ret = objects;
	}
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
	if (nodeType == QUAD_TREE_PARENT){
		topRight->space.render(BLUE);
		topLeft->space.render(BLUE);
		bottomLeft->space.render(BLUE);
		bottomRight->space.render(BLUE);
		
		topRight->render();
		topLeft->render();
		bottomLeft->render();
		bottomRight->render();
	}
	
	else if(nodeType == QUAD_TREE_LEAF){
		for(int i = 0; i < objects.size(); i++)	objects[i].render(BLACK);
	}
	
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

std::vector<Line2D> QuadTree::query(const Point2D & p)
{
	std::vector<Line2D> ret;
	root->query(p, ret);
	return ret;
}

std::vector<Line2D> QuadTree::query(const Point2D & p,const Circle2D c){
	vector<Line2D> ret;
	vector<Point2D> points;
	vector<Line2D> ret_temp;
	
	
	points.push_back(Point2D(c.p.x + c.radius,c.p.y + c.radius ));
	points.push_back(Point2D(c.p.x - c.radius,c.p.y + c.radius ));
	points.push_back(Point2D(c.p.x + c.radius,c.p.y - c.radius ));
	points.push_back(Point2D(c.p.x - c.radius,c.p.y - c.radius ));
	
	for(int i = 0 ; i < points.size(); i++){
		
		ret_temp = query(points[i]);
		
		for(int j = 0; j < ret_temp.size(); j++){
			ret.push_back(ret_temp[j]);
		}	
	}
	return ret;
	
}

void QuadTree::render() const
{
	root->render();
}

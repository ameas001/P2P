// Name: Aranya Meas
// Quarter, Year: Spring 2014
// Lab: 5
//
// This file is to be modified by the student.
// main.cpp
////////////////////////////////////////////////////////////
#include "object.h"
#include "netp2p.h"
#include "quadtree.h"
#include <algorithm>
#include <GL/glut.h>
#include <sstream>

using namespace std;

const int WINDOW_WIDTH = 800;
const int WINDOW_HEIGHT = 800;
const double VIEW_LEFT = 0.0;
const double VIEW_RIGHT = WINDOW_WIDTH;
const double VIEW_TOP = 0.0;
const double VIEW_BOTTOM = WINDOW_HEIGHT;
const int DEPTH_LIMIT = 5;
const int LIST_LIMIT = 1;

QuadTree quadtree(Rect2D(Point2D(VIEW_LEFT, VIEW_TOP), VIEW_RIGHT - VIEW_LEFT, VIEW_BOTTOM - VIEW_TOP), DEPTH_LIMIT, LIST_LIMIT);

int currentTime = 0;
int previousTime = 0;

Point2D p1 = Point2D(450.0, 250.0);
Circle2D c1 = Circle2D(p1, 40.0, RED, true);
Player me = Player(c1, 1.0, 1.0);

Color col(0.0, 0.5, 0.0);

vector<Line2D>  lines;
NetP2P* callerg = new NetP2P(5133, "127.0.0.1");

void GLrender();
void GLupdate();
void qtreeinit();

//Initializes OpenGL attributes
void GLInit(int* argc, char** argv)
{
	glutInit(argc, argv);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE);
	glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);
	glutCreateWindow("CS 134 - Lab2");
	glutDisplayFunc(GLrender);
	glutIdleFunc(GLupdate);
	glClearColor(1.0f, 1.0f, 1.0f, 0.0f);
	gluOrtho2D(VIEW_LEFT, VIEW_RIGHT, VIEW_BOTTOM, VIEW_TOP);
	qtreeinit();
}

void key_up(unsigned char key, int x, int y)
{
  switch(key)
  {
    case 'w':
      me.down = false;
      break;
    case 'a':
      me.left = false;
      break;
    case 's':
      me.up = false;
      break;
    case 'd':
      me.right = false;
      break;
    default:
      break;
  }
}

void glKeyboard(unsigned char key, int x, int y)
{
  switch(key)
  {
    case 'w':
      me.down = true;
      break;
    case 'a':
      me.left = true;
      break;
    case 's':
      me.up = true;
      break;
    case 'd':
      me.right = true;
      break;
    default:
      break;
  }
}

int main(int argc, char** argv)
{
	GLInit(&argc, argv);
  glutKeyboardFunc(glKeyboard);
  glutKeyboardUpFunc(key_up);
	glutMainLoop();
	return 0;
}

void qtreeinit()
{
	quadtree.insert(Line2D(Point2D(100,100),Point2D(100,150)));
	quadtree.insert(Line2D(Point2D(300,100),Point2D(300,150)));
	quadtree.insert(Line2D(Point2D(450,320),Point2D(600,320)));
	quadtree.insert(Line2D(Point2D(50,360),Point2D(50,385)));
	quadtree.insert(Line2D(Point2D(150,360),Point2D(150,398)));
	quadtree.insert(Line2D(Point2D(150,405),Point2D(250,405)));
	quadtree.insert(Line2D(Point2D(150,760),Point2D(150,785)));
}

void collisions()
{
  if(me.c.p.x + me.c.radius > WINDOW_WIDTH)
  {
    me.vx = -me.vx;
    me.c.p.x = WINDOW_WIDTH - me.c.radius;
  }
  else if(me.c.p.x - me.c.radius < 0)
  {
  	me.vx = -me.vx;
    me.c.p.x = 0 + me.c.radius;
  }
  else if(me.c.p.y + me.c.radius > WINDOW_HEIGHT) 
  {
    me.vy = -me.vy;
    me.c.p.y = WINDOW_HEIGHT - me.c.radius;
  }
  else if(me.c.p.y - me.c.radius < 0)
  {
  	me.vy = -me.vy;
    me.c.p.y = 0 + me.c.radius;
  }
}

void caller()
{
	int datasent = 0;
	
	int x, y;
	x = me.c.p.x;
	y = me.c.p.y;
	
	string s1 = static_cast<ostringstream*>( &(ostringstream() << x) )->str();
	string s2 = static_cast<ostringstream*>( &(ostringstream() << y) )->str();
	
	string sol = s1 + "," + s2;
	
	char* buf = (char*) malloc(256);
	for(int i = 0; i < sol.size(); i++)
		buf[i] = sol[i];
	
	datasent = callerg->writeBuffer(buf); 
}

void GLupdate()
{
	const int FRAME_RATE = 25;

	glutPostRedisplay();

	//sleep is not effective in capturing constant time between frames because sleep
	//doesn't consider the time it takes for context-switching. However, this reduces
	//the cpu-usage. If accurate time frames are desire, use a time accumulator
	currentTime = glutGet(GLUT_ELAPSED_TIME);
	int diffTime = currentTime - previousTime;
	previousTime = currentTime;
	usleep(1000 * std::max(FRAME_RATE - diffTime, 0));
	
	double vx = 0;
	double vy = 0;
	
	if(me.up) vy += .1;
	if(me.down) vy -= .1;
	if(me.right) vx += .1;
	if(me.left) vx -= .1;
	
  me.update_NOW(vx, vy);
  caller();
  collisions(); 
}

void GLrender()
{
	glClear(GL_COLOR_BUFFER_BIT);
	lines = quadtree.query(Point2D(me.c.p.x,me.c.p.y),me.c);
	quadtree.render();
  me.c.render(col);
	glFlush();	
	glutSwapBuffers();
}

//object.cpp
//
//You do not need to modify this file
//C.P
////////////////////////////////////////////////////////////
#include "object.h"

#include "const.h"
#include <cmath>
#include <algorithm>
#include <GL/glut.h>

void Object2D::render() const
{
	glColor4d(color.r, color.g, color.b, color.a);
}

void Object2D::render(const Color & col) const
{
	glColor4d(col.r, col.g, col.b, col.a);
}

Point2D::Point2D() : Object2D(), x(0.0), y(0.0) {}
Point2D::Point2D(const double & nx, const double & ny, const Color & col) 
	: Object2D(col), x(nx), y(ny)
{}


double Point2D::magnitude() const
{
	return sqrt(x * x + y * y);
}

double Point2D::magnitude2() const
{
	return x * x + y * y;
}

Point2D Point2D::normalized() const
{
	return (*this) / magnitude();
}

void Point2D::set(const Point2D & pos)
{
	x = pos.x;
	y = pos.y;
}

void Point2D::move(double dx, double dy) 
{
	x += dx;
	y += dy;
}

void Point2D::render() const
{
	Object2D::render();
	glBegin(GL_POINTS);
	glVertex2d(x, y);
	glEnd();
}

void Point2D::render(const Color & col) const
{
	Object2D::render(col);
	glBegin(GL_POINTS);
	glVertex2d(x, y);
	glEnd();
}

Line2D::Line2D() : Object2D(), p1(), p2() {}
Line2D::Line2D(double x1, double y1, double x2, double y2, const Color & col)
	: Object2D(col), p1(x1, y1), p2(x2, y2)
{}
Line2D::Line2D(const Point2D & p1, const Point2D & p2, const Color & col)
	: Object2D(col), p1(p1), p2(p2)
{}

double Line2D::magnitude() const
{
	return sqrt(pow(p2.x - p1.x, 2) + pow(p2.y - p1.y, 2));
}

double Line2D::magnitude2() const
{
	return pow(p2.x - p1.x, 2) + pow(p2.y - p1.y, 2);
}

double Line2D::angle() const
{
	return atan2(p2.y - p1.y, p2.x - p1.x);
}

double Line2D::dot() const
{
	return p1.x * p2.x + p1.y * p2.y;
}

double Line2D::cross() const
{
	return p1.x * p2.y - p2.x * p1.y;
}

Point2D Line2D::delta() const
{
	return p2 - p1;
}

Point2D Line2D::midpoint() const
{
	return (p1 + p2) / 2.0;
}

Rect2D Line2D::boundingRect() const
{
	Point2D corner1 = Point2D(std::min(p1.x, p2.x), std::min(p1.y, p2.y));
	Point2D corner2 = Point2D(std::max(p1.x, p2.x), std::max(p1.y, p2.y));

	return Rect2D(corner1, corner2.x - corner1.x, corner2.y - corner1.y);
}

void Line2D::set(const Point2D & pos) 
{
	Point2D moveBy = pos - midpoint();
	p1 += moveBy;
	p2 += moveBy;
}

void Line2D::move(double dx, double dy)
{
	p1.move(dx, dy);
	p2.move(dx, dy);
}

void Line2D::render() const
{
	Object2D::render();
	glBegin(GL_LINES);
	glVertex2d(p1.x, p1.y);
	glVertex2d(p2.x, p2.y);
	glEnd();
}

void Line2D::render(const Color & col) const
{
	Object2D::render(col);
	glBegin(GL_LINES);
	glVertex2d(p1.x, p1.y);
	glVertex2d(p2.x, p2.y);
	glEnd();
}

Rect2D::Rect2D() : Shape2D(), p(), width(0.0), height(0.0) {}
Rect2D::Rect2D(const Point2D & p, double width, double height, const Color & col, bool fill) 
	: Shape2D(col, fill), p(p), width(width), height(height) 
{}

double Rect2D::perimeter() const
{
	return 2 * width + 2 * height;
}

double Rect2D::area() const
{
	return width * height;
}

Point2D Rect2D::center() const
{
	return Point2D(p.x + width / 2, p.y + height / 2);
}

void Rect2D::set(const Point2D & pos)
{
	p = pos;
}

void Rect2D::move(double dx, double dy)
{
	p.move(dx, dy);
}

void Rect2D::present(bool ignoreParam, bool fil) const
{
	if ((ignoreParam && filled) || (!ignoreParam && fil))
		glBegin(GL_QUADS);
	else
		glBegin(GL_LINE_LOOP);

	glVertex2d(p.x, p.y);
	glVertex2d(p.x + width, p.y);
	glVertex2d(p.x + width, p.y + height);
	glVertex2d(p.x, p.y + height);
	glEnd();
}

void Rect2D::render() const
{
	Object2D::render();
	present(true);
}

void Rect2D::render(const Color & col) const
{
	Object2D::render(col);
	present(true);
}

void Rect2D::render(const Color & col, bool fil) const
{
	Object2D::render(col);
	present(false, fil);
}

Circle2D::Circle2D() : Shape2D(), p(), radius(0.0) {}
Circle2D::Circle2D(const Point2D & p, double radius, const Color & col, bool fill)
	: Shape2D(col, fill), p(p), radius(radius)
{}

double Circle2D::perimeter() const
{
	return 2 * PI * radius;
}

double Circle2D::area() const
{
	return PI * radius * radius;
}

Rect2D Circle2D::boundingRect() const
{
	return Rect2D(Point2D(p.x - radius, p.y - radius), 2 * radius, 2 * radius);
}

void Circle2D::set(const Point2D & pos)
{
	p = pos;
}

void Circle2D::move(double dx, double dy)
{
	p.move(dx, dy);
}

void Circle2D::present(bool ignoreParam, bool fil) const
{
	if ((ignoreParam && filled) || (!ignoreParam && fil))
		glBegin(GL_POLYGON);
	else
		glBegin(GL_LINE_LOOP);
	
	const double ARC_LINE = PI / 180;
	double theta = 0.0;
	while (theta <= 2 * PI)
	{
		glVertex2d(p.x + radius * cos(theta), p.y + radius * sin(theta));
		theta += ARC_LINE;
	}
	glEnd();
}

void Circle2D::render() const
{
	Object2D::render();
	present(true);
}

void Circle2D::render(const Color & col) const
{
	Object2D::render(col);
	present(true);
}

void Circle2D::render(const Color & col, bool fil) const
{
	Object2D::render(col);
	present(false, fil);
}

void Player::render() const
{
  Object2D::render(this->c.color);
  this->c.present();
}

void Player::move(double dx, double dy)
{
  c.move(dx, dy);
}

void Player::change_color()
{
  Color col;
  this->c.color = col;
}

void Player::update_NOW(double x, double y)
{
  vx += x;
  vy += y;
	c.move(vx, vy);
}

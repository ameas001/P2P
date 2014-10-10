//object.h
//
//You do not need to modify this file
//C.P
////////////////////////////////////////////////////////////
#ifndef __OBJECT_H__
#define __OBJECT_H__

#include <iostream>

struct Color
{
	double r;
	double g;
	double b;
	double a;
	Color() : r(0.0), g(0.0), b(0.0), a(0.0) {}
	Color(const double & nr, const double & ng, const double & nb, const double & na = 0.0)
		: r(nr), g(ng), b(nb), a(na)
	{}
};

#define WHITE Color(1.0, 1.0, 1.0)
#define BLACK Color(0.0, 0.0, 0.0)
#define RED Color(1.0, 0.0, 0.0)
#define GREEN Color(0.0, 1.0, 0.0)
#define BLUE Color(0.0, 0.0, 1.0)
#define YELLOW Color(1.0, 1.0, 0.0)
#define PINK Color(1.0, 0.0, 1.0)
#define CYAN Color(0.0, 1.0, 1.0)
#define ORANGE Color(1.0, 0.5, 0.0)
#define VIOLET Color(0.5, 0.0, 1.0)
#define GRAY Color(0.5, 0.5, 0.5)
#define BROWN Color(0.6, 0.3, 0.0)

struct Object2D;
struct Shape2D;
struct Point2D;
struct Line2D;
struct Rect2D;
struct Circle2D;

struct Object2D
{
	Color color;
	Object2D() : color() {}
	Object2D(const Color & c) : color(c) {}
	
	virtual void move(double dx, double dy) = 0;
	virtual void render() const;
	virtual void render(const Color & col) const;

	virtual ~Object2D() {}
};

struct Shape2D : public Object2D
{
	bool filled;
	Shape2D() : Object2D(), filled(false) {}
	Shape2D(const Color & c, bool f = false)
		: Object2D(c), filled(f)
	{}
	
	virtual void present(bool ignoreParam = true, bool fil = false) const = 0;
	virtual void render(const Color & col, bool fil) const = 0;

	virtual ~Shape2D() {}
};

struct Point2D : public Object2D
{
	double x;
	double y;

	Point2D();
	Point2D(const double & nx, const double & ny, const Color & col = WHITE);
	
	Point2D operator+(const Point2D & rhs) const { return Point2D(x + rhs.x, y + rhs.y); }
	Point2D operator-(const Point2D & rhs) const { return Point2D(x - rhs.x, y - rhs.y); }
	Point2D operator*(double val) const { return Point2D(x * val, y * val); }
	Point2D operator/(double val) const { return Point2D(x / val, y / val); }
	Point2D operator+=(const Point2D & rhs) { x += rhs.x; y += rhs.y; return *this; }
	Point2D operator-=(const Point2D & rhs) { x -= rhs.x; y -= rhs.y; return *this; }
	Point2D operator*=(double val) { x *= val; y *= val; return *this; }
	Point2D operator/=(double val) { x /= val; y /= val; return *this; }
	
	double magnitude() const;
	double magnitude2() const;
	Point2D normalized() const;

	void set(const Point2D & pos);
	virtual void move(double dx, double dy);
	virtual void render() const;
	virtual void render(const Color & col) const;

	virtual ~Point2D() {}
};

struct Line2D : public Object2D
{
	Point2D p1;
	Point2D p2;

	Line2D();
	Line2D(double x1, double y1, double x2, double y2, const Color & col = BLUE);
	Line2D(const Point2D & p1, const Point2D & p2, const Color & col = WHITE);
	
	double magnitude() const;
	double magnitude2() const;
	double angle() const;
	double dot() const;
	double cross() const;
	Point2D delta() const;
	Point2D midpoint() const;
	Rect2D boundingRect() const;

	void set(const Point2D & pos);
	virtual void move(double dx, double dy);
	virtual void render() const;
	virtual void render(const Color & col) const;

	virtual ~Line2D() {}
};

struct Rect2D : public Shape2D
{
	Point2D p;
	double width;
	double height;

	Rect2D();
	Rect2D(const Point2D & p, double width, double height, const Color & col = WHITE, bool fill = false);

	double perimeter() const;
	double area() const;
	Point2D center() const;
	
	void set(const Point2D & pos);
	virtual void move(double dx, double dy);
	virtual void present(bool ignoreParam = true, bool fil = false) const;
	virtual void render() const;
	virtual void render(const Color & col) const;
	virtual void render(const Color & col, bool fil) const;

	virtual ~Rect2D() {}
};

struct Circle2D : public Shape2D
{
	Point2D p;
	double radius;

	Circle2D();
	Circle2D(const Point2D & p, double radius, const Color & col = Color(), bool fill = false);

	double perimeter() const;
	double area() const;
	Rect2D boundingRect() const;
	
	void set(const Point2D & pos);
	virtual void move(double dx, double dy);
	virtual void present(bool ignoreParam = true, bool fil = false) const;
	virtual void render() const;
	virtual void render(const Color & col) const;
	virtual void render(const Color & col, bool fil) const;

	virtual ~Circle2D() {}
};

struct Player : public Circle2D
{
	Circle2D c;
  double vx;
  double vy;
  bool up;
  bool down;
  bool left;
  bool right;

  Player() : Circle2D(), vx(), vy() {}
  Player(const Circle2D & cir, double x, double y) : vx(x), vy(y) 
  {
    c = Circle2D(cir.p, cir.radius, cir.color, true);
  }
  
  void update_NOW(double x, double y);
  
  virtual void change_color();
  virtual void render() const;
  virtual void move(double dx, double dy);
  virtual ~Player() {}
};

#endif

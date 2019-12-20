#include <cmath>
#include <sstream>
#include <string>
#include "physics.h"
#ifndef __MATHS_H
#define __MATHS_H

struct Line {
	double x1;
	double x2;
	double y1;
	double y2;
};
struct Square {
	double x1;
	double x2;
	double y1;
	double y2;
};

std::string itoa (int val)
{
	std::stringstream ss;
	ss << val;
	std::string ret;
	ss >> ret;
	return ret;
}
bool intersect (Line l1, Line l2)
{
  double distAB, theCos, theSin, newX, ABpos, Ax = l1.x1, Ay = l1.y1,
	  Bx = l1.x2, By = l1.y2, Cx = l2.x1, Cy = l2.y1, Dx = l2.x2, Dy = l2.y2;

  //  Fail if either line segment is zero-length.
  if (Ax==Bx && Ay==By || Cx==Dx && Cy==Dy) 
	  return false;

  //  Fail if the segments share an end-point.
  if (Ax==Cx && Ay==Cy || Bx==Cx && By==Cy
  ||  Ax==Dx && Ay==Dy || Bx==Dx && By==Dy) {
    return false; }

  //  (1) Translate the system so that point A is on the origin.
  Bx-=Ax; By-=Ay;
  Cx-=Ax; Cy-=Ay;
  Dx-=Ax; Dy-=Ay;

  //  Discover the length of segment A-B.
  distAB=sqrt(Bx*Bx+By*By);

  //  (2) Rotate the system so that point B is on the positive X axis.
  theCos=Bx/distAB;
  theSin=By/distAB;
  newX=Cx*theCos+Cy*theSin;
  Cy  =Cy*theCos-Cx*theSin; Cx=newX;
  newX=Dx*theCos+Dy*theSin;
  Dy  =Dy*theCos-Dx*theSin; Dx=newX;

  //  Fail if segment C-D doesn't cross line A-B.
  if (Cy<0. && Dy<0. || Cy>=0. && Dy>=0.)
	  return false;

  //  (3) Discover the position of the intersection point along line A-B.
  ABpos=Dx+(Cx-Dx)*Dy/(Dy-Cy);

  //  Fail if segment C-D crosses line A-B outside of segment A-B.
  if (ABpos<0. || ABpos>distAB)
	  return false;

  //  (4) Apply the discovered position to line A-B in the original coordinate system.
  /**X=Ax+ABpos*theCos;
  *Y=Ay+ABpos*theSin;*/

  //  Success.
  return true;
}
bool intersectSquare (Line line, Square sq)
{
	double x1 = sq.x1, y1 = sq.y1, x2 = sq.x2, y2 = sq.y2;
	Line left, top, right, bottom;

	left.x1 = x1;
	left.x2 = x1;
	left.y1 = y1;
	left.y2 = y2;

	right.x1 = x2;
	right.x2 = x2;
	right.y1 = y1;
	right.y2 = y2;

	top.x1 = x1;
	top.x2 = x2;
	top.y1 = y2;
	top.y2 = y2;

	bottom.x1 = x1;
	bottom.x2 = x2;
	bottom.y1 = y1;
	bottom.y2 = y1;

	return intersect(line, left) || intersect(line, right) || intersect(line, top) || intersect(line, bottom);
}
const long double PI = 3.14159265358979323846264338327950288419716939937510L;
inline
double distanceToPoint (const Vector2D& p1, const Vector2D& p2)
{
	return sqrt((p2.x - p1.x) * (p2.x - p1.x) + (p2.y - p1.y) * (p2.y - p1.y));
}
inline
double dirToPoint (const Vector2D& p1, const Vector2D& p2)
{
	return atan2(p1.y - p2.y, p2.x - p1.x) * 180 / PI;
}
inline
Vector2D lengthdir (double length, double dir)
{
	return Vector2D(cos(dir * PI / 180) * length, -sin(dir * PI / 180) * length);
}
inline
Vector2D lengthdir (const Vector2D& v1, const Vector2D& v2)
{
	double dir = dirToPoint(v1, v2);
	double len = distanceToPoint(v1, v2);
	return lengthdir(len, dir);
}
#endif
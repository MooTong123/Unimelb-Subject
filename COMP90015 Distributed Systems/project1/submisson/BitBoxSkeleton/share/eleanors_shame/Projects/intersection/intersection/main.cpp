#include <glwin.h>
#include "physics.h"

struct Line {
	double x1;
	double x2;
	double y1;
	double y2;
} line1, line2;
struct Square {
	double x1;
	double x2;
	double y1;
	double y2;
} square;

Vector2D getMousePos ();
void render (GLWindow& win);

bool intersect(Line l1, Line l2)
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
int APIENTRY WinMain (HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	MSG msg;
	bool done = false;

	line1.x1 = 0;
	line1.y1 = 0;

	line2.x1 = 500;
	line2.y1 = 500;
	line2.x2 = 700;
	line2.y2 = 500;

	square.x1 = 700;
	square.y1 = 300;
	square.x2 = 900;
	square.y2 = 500;

	GLWindow myWin(1024, 768, false);
	while (!done) {
		// Window message handling
		if (PeekMessage(&msg, 0, 0, 0, PM_REMOVE)) {
			if (msg.message == WM_QUIT) {
				done = true;
			}
			else {
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
		}
		if (myWin.checkKey(VK_ESCAPE)) {
			done = true;
		}
		else {
			line1.x2 = lengthdir(Vector2D(0, 0), getMousePos()).x;
			line1.y2 = 768 - lengthdir(Vector2D(0, 0), getMousePos()).y;
			if (intersect(line1, line2)) {
				glColor3ub(255, 0, 0);
			}
			else if (intersectSquare(line1, square)) {
				glColor3ub(0, 255, 0);
			}
			else {
				glColor3ub(0, 0, 255);
			}
			render(myWin);
		}
	}
	return 0;
}
Vector2D getMousePos ()
{
	POINT pos;
	GetCursorPos(&pos);
	return Vector2D(pos.x, pos.y);
}
void render (GLWindow& win)
{
	wClear();
	glBegin(GL_LINES);
	glVertex2f(line1.x1, line1.y1);
	glVertex2f(line1.x2, line1.y2);
	glEnd();

	glBegin(GL_LINES);
	glVertex2f(line2.x1, line2.y1);
	glVertex2f(line2.x2, line2.y2);
	glEnd();

	glBegin(GL_LINES);
	glVertex2f(square.x1, square.y1);
	glVertex2f(square.x1, square.y2);
	glEnd();
	glBegin(GL_LINES);
	glVertex2f(square.x2, square.y1);
	glVertex2f(square.x2, square.y2);
	glEnd();
	glBegin(GL_LINES);
	glVertex2f(square.x1, square.y1);
	glVertex2f(square.x2, square.y1);
	glEnd();
	glBegin(GL_LINES);
	glVertex2f(square.x1, square.y2);
	glVertex2f(square.x2, square.y2);
	glEnd();

	win.wSwapBuffers();
}

#include <ctime>
#include <Windows.h>
#include <gl/GL.h>
#include <gl/glu.h>
#include "physics.h"
#include "object.h"
#include <glwin.h>

void eglRectangle (double x, double y, double width, double height);
class Square: public Mass
{
	public:
		Square (double mass, double friction)
		{
			m = mass;
			f = friction;
		}
		Square () {}
		void solve ()
		{
			wrap(state.pos, 1024, 768);
		}
};
class Ball: public Mass
{
	public:
		Ball ()
		{
			m = 1;
			f = 0;
			state.pos.x = 320;
			state.pos.y = 900;
			add(50, 0);
		}
		Vector2D acceleration (const State& initial, double m)
		{
			const float k = 5;
			const float b = 0;
			return Vector2D(state.force.x, (-k * initial.pos.y - b * initial.vel.y) / 100);
		}
		void solve ()
		{
			;
		}
};
class Control: public Object
{
	public:
		Control (): win(&GLWindow::getWin()), obj0(1, 2.5), speed(10)
		{
		}

		void update (long double dt)
		{
			if (obj1.getx() > 1008) {
				obj1.add(-100, 0);
			}
			if (obj1.getx() < 0) {
				obj1.add(100, 0);
			}
			if (win->checkKey(VK_LEFT)) {
				obj0.apply(Vector2D(-speed, 0));
			}
			if (win->checkKey(VK_RIGHT)) {
				obj0.apply(Vector2D(speed, 0));
			}
			if (win->checkKey(VK_UP)) {
				obj0.apply(Vector2D(0, speed));
			}
			if (win->checkKey(VK_DOWN)) {
				obj0.apply(Vector2D(0, -speed));
			}
			obj0.update(dt);
			obj1.update(dt);
		}
		void render (long double dt)
		{
			
			glColor3ub(255, 0, 0);
			eglRectangle(obj0.getx(dt), obj0.gety(dt), 32, 32);
			glColor3ub(0, 0, 255);
			eglRectangle(obj1.getx(dt), abs(obj1.gety(dt)) * 0.7, 16, 16);
		}

	private:
		Square obj0;
		Ball obj1;
		GLWindow* win;
		const double speed;
};
const double slowRatio = 100;
const double dt = 0.01;
int APIENTRY WinMain (HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	MSG msg;
	bool done = false;

	GLWindow myWin(1024, 768, false);
	Object::addObject<Control>();
	double oldtime = clock();
	double newtime = oldtime;
	double accumulator = 0.0;

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
			// do physics
			newtime = clock();
			double frametime = newtime - oldtime;
			frametime /= slowRatio;
			oldtime = newtime;

			// Make sure dt is precise
			for (accumulator += frametime; accumulator >= dt; accumulator -= dt) {
				Object::updateAll(dt);
			}
		
			wClear();
			
			Object::renderAll(accumulator / dt);

			myWin.wSwapBuffers();
		}
	}

	return 0;
}
void eglRectangle (double x, double y, double width, double height)
{
	glBegin(GL_QUADS);
	glVertex2f(x, y);
	glVertex2f(x + width, y);
	glVertex2f(x + width, y + height);
	glVertex2f(x, y + height);
	glEnd();
}
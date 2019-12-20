#include <ctime>
#include <Windows.h>
#include <gl/GL.h>
#include <gl/glu.h>
#include "physics.h"
#include <glwin.h>
class Square: public GLMass
{
	public:
		Square (GLWindow& window, double mass, double friction): win(window)
		{
			m = mass;
			f = friction;
		}
		void solve ()
		{
			if (win.checkKey(VK_LEFT)) {
				apply(Vector2D(-10, 0));
			}
			if (win.checkKey(VK_RIGHT)) {
				apply(Vector2D(10, 0));
			}
			if (win.checkKey(VK_UP)) {
				apply(Vector2D(0, 10));
			}
			if (win.checkKey(VK_DOWN)) {
				apply(Vector2D(0, -10));
			}
			wrap(state.pos, 640, 480);
		}
		void render ()
		{
			glBegin(GL_QUADS);
			glColor3f(1.0f, 0.0f, 0.0f);
			glVertex2f(getx(), gety());
			glVertex2f(getx() + 32, gety());
			glVertex2f(getx() + 32, gety() + 32);
			glVertex2f(getx(), gety() + 32);
			glEnd();
		}

	private:
		Square(): win(*((GLWindow*)0)) {}
		GLWindow& win;
};

const double slowRatio = 100;
const double dt = 0.01;

void displayScene (double alpha);
int APIENTRY WinMain (HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	MSG msg;
	bool done = false;

	GLWindow myWin(false);
	Square obj(myWin, 1, 2.5);
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
				obj.update(dt);
			}
		
			wClear();
			obj.draw(accumulator / dt);
			myWin.wSwapBuffers();
		}
	}

	return 0;
}
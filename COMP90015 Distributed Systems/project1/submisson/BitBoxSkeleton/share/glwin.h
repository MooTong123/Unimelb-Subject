#ifndef __GLWIN_H
#define __GLWIN_H
#include <windows.h>
#include <gl/GL.h>
#include <gl/GLU.h>

#pragma comment (lib, "opengl32.lib")
#pragma comment (lib, "glu32.lib")
#ifdef _DEBUG
#pragma comment (lib, "glwindbg.lib")
#else
#pragma comment (lib, "glwin.lib")
#endif

class GLWindow
{
	public:
		GLWindow::GLWindow (int width = 1440, int height = 900, bool fullscreen = true, bool setOwner = true);
		~GLWindow ();

		void activate () { active = this; }
		bool checkKey (int key) const { return keys[key]; }

		GLvoid wSetWin (GLWindow& win) { cPtr = &win; }

		GLvoid wSwapBuffers () { SwapBuffers(hDC); }

		static GLWindow& getWin () { return *active; }

	private:
		HGLRC		hRC;			// Permanent rendering context
		HDC			hDC;			// Private GDI device context
		HWND		hWnd;
		HINSTANCE	hInstance;


		bool		keys[256];
		bool		win_active;
		bool		fullscreen;

		static LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
		static GLvoid wResizeGlScene (GLsizei width, GLsizei height);
		static GLWindow* cPtr;

		GLvoid wInitGL ();
		GLvoid wCreateWin (const char* title, GLint width, GLint height, GLint bits);
		GLvoid wSetupWin (GLint bits);
		GLvoid wMakeFullscreen (GLint width, GLint height, GLint bits);

		static GLWindow* active;
};
GLWindow* GLWindow::active = nullptr;
void wClear ();
#ifdef __PHYSICS_H
class GLMass: public Mass
{
	public:
		GLMass (GLWindow& window): win(&window) {}
		GLMass () {}
		
	protected:
		GLWindow* win;
};
#endif
#endif
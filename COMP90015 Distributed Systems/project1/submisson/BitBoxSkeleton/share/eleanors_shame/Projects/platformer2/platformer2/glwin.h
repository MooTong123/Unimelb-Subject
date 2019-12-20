#ifndef __GLWIN_H
#define __GLWIN_H
#include <vector>
#include <windows.h>
#include <gl/GL.h>
#include <gl/GLU.h>
#pragma comment (lib, "glu32.lib")
#pragma comment (lib, "opengl32.lib")

#undef MB_RIGHT
const int MB_LEFT = 0;
const int MB_RIGHT = 0;

typedef GLuint Font;

struct Key {
	bool down;
	bool pressed;
	bool released;
	void operator = (bool x) { down = x; }
	operator bool () const { return down; }
};
class GLWindow
{
	public:
		~GLWindow ();
		bool getKey (int key) const { return keys[key].down; }
		bool getKeyPressed (int key) const { return keys[key].pressed; }
		bool getKeyReleased (int key) const { return keys[key].released; }
		bool checkMouse (int button) const { return mouse[button]; }

		Font makeFont (int height, int width, int weight, const char* name);
		void wDrawFont (Font font, const char* string, double x, double y);
		void wDrawFont (Font font, const std::string& string, double x, double y) { wDrawFont(font, string.c_str(), x, y); }

		HDC getHdc () const { return hDC; }

		GLvoid wSwapBuffers () { SwapBuffers(hDC); }

		static GLWindow* getInstance (int width = 1440, int height = 900, bool fullscreen = true);

		void update ();

	private:
		HGLRC		hRC;			// Permanent rendering context
		HDC			hDC;			// Private GDI device context
		HWND		hWnd;
		HINSTANCE	hInstance;


		Key			keys[256];
		bool		mouse[2];
		bool		win_active;
		bool		fullscreen;

		std::vector<GLuint> fonts;

		static LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
		static GLvoid wResizeGlScene (GLsizei width, GLsizei height);

		GLWindow::GLWindow (int width = 1440, int height = 900, bool fullscreen = true);

		GLvoid wInitGL ();
		GLvoid wCreateWin (const char* title, GLint width, GLint height, GLint bits);
		GLvoid wSetupWin (GLint bits);
		GLvoid wMakeFullscreen (GLint width, GLint height, GLint bits);
};
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
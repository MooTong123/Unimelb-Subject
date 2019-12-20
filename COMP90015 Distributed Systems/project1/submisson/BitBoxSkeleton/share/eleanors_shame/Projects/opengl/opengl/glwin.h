#include <windows.h>
#include <gl/GL.h>
#include <gl/GLU.h>

#pragma comment (lib, "opengl32.lib")
#pragma comment (lib, "glu32.lib")

class GLWindow
{
	public:
		GLWindow::GLWindow (bool fullscreen = true, bool setOwner = true);
		~GLWindow ();

		bool is_active () const { return win_active; }
		bool checkKey (int key) const { return keys[key]; }

		GLvoid wSetWin (GLWindow& win) { cPtr = &win; }

		GLvoid wSwapBuffers () { SwapBuffers(hDC); }

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
};
void wClear ();
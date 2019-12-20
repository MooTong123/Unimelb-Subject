#include "glwin.h"
#include <cstring>
#include <Windows.h>
#include <gl/GL.h>
#include <gl/GLU.h>

GLWindow::GLWindow (int width, int height, bool fullscreen): fullscreen(fullscreen), win_active(true)
{
	for (int i = 0; i < 256; ++i) {
		keys[i].down = false;
		keys[i].pressed = false;
		keys[i].released = false;
	}
	mouse[0] = false;
	mouse[1] = false;
	wCreateWin("OpenGL", width, height, 16);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(0, width, 0, height);
	glMatrixMode(GL_MODELVIEW);
}
GLWindow::~GLWindow ()
{
	if (fullscreen) {
		ChangeDisplaySettings(nullptr, 0);
		ShowCursor(true);
	}
	if (hRC) {
		if (!wglMakeCurrent(nullptr, nullptr)) {
			throw 0;
		}
		if (!wglDeleteContext(hRC)) {
			throw 1;
		}
	}
	if (hRC) {
		if (!ReleaseDC(hWnd, hDC)) {
			throw 2;
		}
	}
	for (unsigned int i = 0; i < fonts.size(); ++i) {
		glDeleteLists(fonts[i], 96);
	}
}

Font GLWindow::makeFont (int height, int width, int weight, const char* name)
{
	HFONT font;
	HFONT oldfont;
	fonts.push_back(glGenLists(96));
	font = CreateFont(-height, width, 0, 0, weight, false, false, false, ANSI_CHARSET, OUT_TT_PRECIS, CLIP_DEFAULT_PRECIS, ANTIALIASED_QUALITY,
					  FF_DONTCARE | DEFAULT_PITCH, name);
	oldfont = (HFONT)SelectObject(hDC, font);
	wglUseFontBitmaps(hDC, 32, 96, fonts[fonts.size() - 1]);
	SelectObject(hDC, oldfont);
	DeleteObject(font);

	return fonts.size() - 1;
}
void GLWindow::wDrawFont (Font font, const char* string, double x, double y)
{
	glRasterPos2d(x, y);
	glPushAttrib(GL_LIST_BIT);
	glListBase(fonts[font] - 32);
	glCallLists(strlen(string), GL_UNSIGNED_BYTE, string);
	glPopAttrib();
}
void GLWindow::update ()
{
	for (int i = 0; i < 256; ++i) {
		keys[i].pressed = false;
		keys[i].released = false;
	}
}

GLvoid GLWindow::wResizeGlScene (GLsizei width, GLsizei height)
{
	if (height == 0) {
		height = 1;
	}
	glViewport(0, 0, width, height);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	gluPerspective(45.0, (GLdouble)width / height, 0.1, 100.0);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}
GLvoid GLWindow::wInitGL ()
{
	glShadeModel(GL_SMOOTH);
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
	
	glClearDepth(1.0f);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);

	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
}
GLvoid GLWindow::wCreateWin (const char* title, GLint width, GLint height, GLint bits)
{
	RECT windowRect;
	windowRect.left = 0;
	windowRect.top = 0;
	windowRect.right = width;
	windowRect.bottom = height;

	hInstance = GetModuleHandle(nullptr);

	WNDCLASS wc;
	wc.style			= CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
	wc.lpfnWndProc		= WndProc;
	wc.cbClsExtra		= 0;
	wc.cbWndExtra		= 0;
	wc.hInstance		= hInstance;
	wc.hIcon			= LoadIcon(nullptr, IDI_WINLOGO);
	wc.hCursor			= LoadCursor(nullptr, IDC_ARROW);
	wc.hbrBackground	= nullptr;
	wc.lpszMenuName		= nullptr;
	wc.lpszClassName	= "OpenGL";

	if (!RegisterClass(&wc)) {
		throw 0;
	}
	
	DWORD exStyle;
	DWORD style;
	if (fullscreen) {
		wMakeFullscreen(width, height, bits);
		exStyle = WS_EX_APPWINDOW;
		style = WS_POPUP;
		ShowCursor(false);
	}
	else {
		exStyle = WS_EX_APPWINDOW | WS_EX_WINDOWEDGE;
		style = WS_OVERLAPPEDWINDOW;
	}
	AdjustWindowRectEx(&windowRect, style, false, exStyle);

	if (!(hWnd = CreateWindowEx(	exStyle,
									"OpenGL",
									title,
									WS_CLIPSIBLINGS |
									WS_CLIPCHILDREN |
									style,
									0, 0,
									width,
									height,
									nullptr,
									nullptr,
									hInstance,
									nullptr))) {
		throw 1;
	}

	wSetupWin(bits);
	wResizeGlScene(width, height);
	wInitGL();
}
GLvoid GLWindow::wSetupWin (GLint bits)
{
	GLuint PixelFormat;
	static PIXELFORMATDESCRIPTOR pfd =
	{
		sizeof(PIXELFORMATDESCRIPTOR),
		1,
		PFD_DRAW_TO_WINDOW |
		PFD_SUPPORT_OPENGL |
		PFD_DOUBLEBUFFER,
		PFD_TYPE_RGBA,
		bits,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		16,
		0, 0,
		PFD_MAIN_PLANE,
		0, 0, 0, 0
	};

	if (!(hDC = GetDC(hWnd))) {
		throw 0;
	}
	if (!(PixelFormat = ChoosePixelFormat(hDC, &pfd))) {
		throw 1;
	}
	if (!SetPixelFormat(hDC, PixelFormat, &pfd)) {
		throw 2;
	}
	if (!(hRC = wglCreateContext(hDC))) {
		throw 3;
	}
	if (!wglMakeCurrent(hDC, hRC)) {
		throw 4;
	}
	ShowWindow(hWnd, SW_SHOW);
	SetForegroundWindow(hWnd);
	SetFocus(hWnd);
}
GLvoid GLWindow::wMakeFullscreen(GLint width, GLint height, GLint bits)
{
	DEVMODE	screenSettings;
	memset(&screenSettings, 0, sizeof(screenSettings));

	screenSettings.dmSize		= sizeof(screenSettings);
	screenSettings.dmPelsWidth	= width;
	screenSettings.dmPelsHeight = height;
	screenSettings.dmBitsPerPel = bits;
	screenSettings.dmFields		= DM_BITSPERPEL | DM_PELSWIDTH | DM_PELSHEIGHT;
	if (ChangeDisplaySettings(&screenSettings, CDS_FULLSCREEN) != DISP_CHANGE_SUCCESSFUL) {
		throw 0;
	}
}
GLWindow* GLWindow::getInstance (int width, int height, bool fullscreen)
{
	static GLWindow win(width, height, fullscreen);
	return &win;
}
LRESULT CALLBACK GLWindow::WndProc (HWND hWnd, UINT msg, WPARAM wP, LPARAM lP)
{
	GLWindow* cPtr = GLWindow::getInstance();
	switch (msg) {
		case WM_SYSCOMMAND:
			switch (wP) {
				case SC_SCREENSAVE:
				case SC_MONITORPOWER:
					return 0;
			}
			break;
		case WM_LBUTTONDOWN:
			cPtr->mouse[MB_LEFT] = true;
			break;
		case WM_RBUTTONDOWN:
			cPtr->mouse[MB_RIGHT] = true;
			break;
		case WM_LBUTTONUP:
			cPtr->mouse[MB_LEFT] = false;
			break;
		case WM_RBUTTONUP:
			cPtr->mouse[MB_RIGHT] = false;
			break;
		case WM_KEYDOWN:
			cPtr->keys[wP].down = true;
			cPtr->keys[wP].pressed = true;
			break;
		case WM_KEYUP:
			cPtr->keys[wP].down = false;
			cPtr->keys[wP].released = true;
			break;
		case WM_SIZE:
			wResizeGlScene(LOWORD(lP), HIWORD(lP));
			break;
	}
	return DefWindowProc(hWnd, msg, wP, lP);
}

void wClear ()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}
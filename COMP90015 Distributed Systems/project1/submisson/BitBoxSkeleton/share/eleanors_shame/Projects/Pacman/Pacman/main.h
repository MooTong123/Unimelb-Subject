/***********************************************************************************************************************************************
*	dxbase.h																															   *
*	Author: PlasticineGuy																													   *
*	Description: Contains definitions, includes, libraries and macros for the base of a DX game.														   *
***********************************************************************************************************************************************/

#ifndef __DXBASE
#define __DXBASE
#define DIRECTINPUT_VERSION		0x0800

/* Includes */
#include <windows.h>
#include <commctrl.h>
#include <mmsystem.h>
#include <d3d9.h>
#include <d3dx9.h>
#include <dinput.h>
#include <colour.h>
#include <cstdio>
#include <ctime>
#include <cstring>


/* Libraries */
#pragma comment (lib, "d3d9.lib")
#pragma comment (lib, "d3dx9.lib")
#pragma comment (lib, "d3dx9d.lib")
#pragma comment (lib, "dinput8.lib")
#pragma comment (lib, "dxguid.lib")
#pragma comment (lib, "winmm.lib")

/* Header variables */
#define WIN32_LEAN_AND_MEAN

/* Typedefs */
typedef LPCSTR string;

/* Constants */
#define SCREEN_WIDTH	    640
#define SCREEN_HEIGHT		520
#define SND_CUSTOM			SND_FILENAME | SND_ASYNC
#define SND_CUSTOM2			SND_FILENAME | SND_SYNC
#ifndef pi
#define pi				3.141592653589793238462
#endif //#ifndef pi

#define sprBat				"C:\\C\\sprites\\pong\\bat.png"
#define sprBall				"C:\\C\\sprites\\pong\\ball.png"

/* Macros */
#define d3dBeginScene()			d3dDevice ->BeginScene(); if(SCREEN_REFRESH) {d3dDevice ->Clear(0, 0, D3DCLEAR_TARGET, D3DCOLOR_XRGB(0, 0, 0), 1.0f, 0);}
#define d3dEndScene()			d3dDevice ->EndScene(); d3dDevice ->Present(0, 0, 0, 0);
#define HandleMessage()			if(PeekMessage(&Msg, 0, 0, 0, PM_REMOVE)) { TranslateMessage(&Msg);DispatchMessage(&Msg); }
#define KeyCheck(key)			(keystate[key] & 0x80)
#define AlarmCheck(AlarmNo)		alarm[AlarmNo] <= 0
#define ZeroMemEx(var)			ZeroMemory(&var, sizeof(var))
#define sqr(x)					x * x
#define mean(x, y)				((x + y) / 2)
#define DoObjFunc(fnc)			for(i = 0; bats[i] != 0; i++) {bats[i] -> fnc;}for(i = 0; balls[i] != 0; i++) {balls[i] -> fnc;}

/* Prototypes */
LRESULT CALLBACK	  WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
HRESULT			      InitDX(HWND hwnd, HINSTANCE hInstance);
HRESULT				  InitD3D(HWND hwnd);
VOID				  InitDInput(HWND hwnd, HINSTANCE hInstance);
VOID				  CleanDX(void);
VOID				  CleanD3D(void);
VOID				  CleanDInput(void);
VOID				  DetectInput(void);
INT					  sign(int n);
#endif //ifndef dxbase

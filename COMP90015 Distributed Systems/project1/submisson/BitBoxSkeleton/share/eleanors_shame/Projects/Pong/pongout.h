/***********************************************************************************************************************************************
*	pongout.h																															   *
*	Author: PlasticineGuy																													   *
*	Description: Contains definitions, includes, libraries and macros for Pongout.														   *
***********************************************************************************************************************************************/

#ifndef PONGOPONGOMAINH
	#define PONGOPONGOMAINH
	#define DIRECTINPUT_VERSION		0x0800
	#define PONGOPONGO_VERSION_NAME  "Pongout Version 2.0"

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
	#pragma comment (lib, "dinput8.lib")
	#pragma comment (lib, "dxguid.lib")
	#pragma comment (lib, "winmm.lib")

	/* Header variables */
	#define WIN32_LEAN_AND_MEAN

	/* Constants */
	#define SCREEN_WIDTH	    640
	#define SCREEN_HEIGHT		480
	#define SCORE_LIMIT			4
	#define ENEMY_MAXSPEED		KEYBOARD_SENSITIVITY / ENEMY_DIFFICULTY
	#define INCREASE_SPEED		0.5f
	#define WEARDOWN			75
	#define SND_CUSTOM			SND_FILENAME | SND_ASYNC
	#define SND_CUSTOM2			SND_FILENAME | SND_SYNC
	#define MAX_BRICK_COUNT		100
	#define IDI_ICON1           "sprites\\ppicobig.ico"
	#define IDI_ICON2           "sprites\\ppicosmall.ico"
	#ifndef pi
		#define pi				3.141592653589793238462
	#endif

		#define BATSPR 				"sprites\\bat.png"
		#define BALLSPR				"sprites\\ball.png"
		#define COURTSPR			"sprites\\court.png"
		#define HITSND01			"sounds\\hit01.wav"
		#define HITSND02			"sounds\\hit02.wav"
		#define BRICK01SPR			"sprites\\brick0.png"

	/* Macros */
	#define d3dBeginScene()			d3dDevice ->BeginScene(); if(SCREEN_REFRESH) {d3dDevice ->Clear(0, 0, D3DCLEAR_TARGET, D3DCOLOR_XRGB(0, 0, 0), 1.0f, 0);}
	#define d3dEndScene()			d3dDevice ->EndScene(); d3dDevice ->Present(0, 0, 0, 0);
	#define HandleMessage()			if(PeekMessage(&Msg, 0, 0, 0, PM_REMOVE)) { TranslateMessage(&Msg);DispatchMessage(&Msg); }
	#define KeyCheck(key)			(keystate[key] & 0x80)
	#define AlarmCheck(AlarmNo)		alarm[AlarmNo] <= 0
	#define ZeroMemEx(var)			ZeroMemory(&var, sizeof(var))
	#define ExCleanDX()				texBat ->Release();texBall ->Release();sprBat ->Release();sprBall ->Release();texCourt ->Release();sprCourt ->Release();
	#define sqr(x)					x * x
	#define mean(x, y)				((x + y) / 2)
	#define SOUND01()				PlaySound(HITSND02, 0, SND_CUSTOM2)
	#define	SOUND02()				PlaySound(HITSND01, 0, SND_CUSTOM2)
	#define SOUND03()				Sleep(500);SOUND01();Sleep(200);SOUND02();SOUND02();Sleep(100);SOUND01();Sleep(100);SOUND02();Sleep(250);SOUND01();Sleep(100);SOUND01();
	#define SOUND04()				Sleep(500);SOUND01();Sleep(200);SOUND01();SOUND02();SOUND02();SOUND02();SOUND01();
	#define ballSpeed				mean(ballHSpeed, ballVSpeed)

	/* Prototypes */
	LRESULT CALLBACK	  WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
	HRESULT			      InitDX(HWND hwnd, HINSTANCE hInstance);
	HRESULT				  InitD3D(HWND hwnd);
	VOID				  InitDInput(HWND hwnd, HINSTANCE hInstance);
	VOID				  CleanDX(void);
	VOID				  CleanD3D(void);
	VOID				  CleanDInput(void);
	VOID				  DetectInput(void);
	VOID				  SetDirection(float* HV, float* VV);
	INT					  sign(int n);
	VOID				  ShowText(D3DCOLOR colour, char* text, LONG left, LONG right, LONG top, LONG bottom);
	VOID				  ShowText(D3DCOLOR colour, char* text);
	VOID				  ShowText(D3DCOLOR colour, char* text, LONG left, LONG right);
	FLOAT				  LoadLevel(void);
#endif

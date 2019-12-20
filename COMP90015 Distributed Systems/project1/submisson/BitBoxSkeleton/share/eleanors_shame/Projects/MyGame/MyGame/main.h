#pragma once
#ifndef GAMEMAINH
	#define GAMEMAINH 0xAAAAAAAA

	/* Includes */
	#include <windows.h>
	#include <d3d9.h>
	#include <d3dx9.h>
	#include <dinput.h>

	/* Constants */
	#define SCREEN_WIDTH	    256
	#define SCREEN_HEIGHT		512
	#define BLOCK01				"C:\\C\\sprites\\blockness\\sprBlock01.png"

	/* Macros */
	#define d3dBeginScene()			d3dDevice ->BeginScene(); if(SCREEN_REFRESH) {d3dDevice ->Clear(0, 0, D3DCLEAR_TARGET, D3DCOLOR_XRGB(0, 0, 0), 1.0f, 0);}
	#define d3dEndScene()			d3dDevice ->EndScene(); d3dDevice ->Present(0, 0, 0, 0);
	#define StopBlockMove()			Blocks[CURRENTBLOCK] ->canMove = 0;
	#define CURRENTBLOCK			BlockCount - 1
	#define CollisionGridShorthand	CollisionGrid[(int)(Blocks[CURRENTBLOCK] ->blockpos.x / 16)][(int)(Blocks[CURRENTBLOCK] ->blockpos.y / 16)]
	#define KeyCheck(key)		keystate[key] & 0x80
	#define AlarmCheck(AlarmNo) alarm[AlarmNo] == 0

	/* Libraries */
	#pragma comment (lib, "d3d9.lib")
	#pragma comment (lib, "d3dx9.lib")
	#pragma comment (lib, "dinput8.lib")
	#pragma comment (lib, "dxguid.lib")

	/* Disable deprecation warnings */
	#pragma warning(disable:4996)
#endif

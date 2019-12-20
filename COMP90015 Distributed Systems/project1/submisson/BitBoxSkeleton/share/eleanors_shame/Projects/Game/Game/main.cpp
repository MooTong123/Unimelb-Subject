#include <windows.h>
#include <windowsx.h>
#include <commctrl.h>
#include "resource.h"
#include <d3d9.h>
#include <dinput.h>
#pragma comment (lib, "d3d9.lib")
#pragma comment (lib, "dinput8.lib")
#pragma comment (lib, "dxguid.lib")
#define SCREEN_WIDTH 1440
#define SCREEN_HEIGHT 900
#define CUSTOMFVF (D3DFVF_XYZ | D3DFVF_DIFFUSE)
#define KeyState(x) keystate[x] & 0x80
#define COL_WHITE D3DCOLOR_COLORVALUE( 1.0, 1.0, 1.0, 1.0 )
const char g_szClassName[] = "myWindowClass";
HWND g_hToolbar = NULL;
int g_Destroy = 0;
float speed = 5.0;
LPDIRECT3D9 d3d = 0;
LPDIRECT3DDEVICE9 d3ddev = 0;
LPDIRECT3DVERTEXBUFFER9 v_buffer = NULL;
LPDIRECTINPUT8 din;
LPDIRECTINPUTDEVICE8 keyb;
BYTE keystate[256];
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
void InitDX(HINSTANCE, HWND);
void InitDI(HINSTANCE, HWND);
void InitD3D(HWND hwnd);

void RenderTriangle(float, float);
void RenderRectangle(float, float, float, float);

void DetectInput(void);

void CleanD3D(void);
void CleanDI(void);
void CleanDX(void);
struct CUSTOMVERTEX {
	float x, y, z;
	DWORD colour;
};

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {

	WNDCLASSEX wc;
	HWND hwnd;
	MSG Msg;

	float x = 200, y = 200;

	wc.cbSize = sizeof(WNDCLASSEX);
	wc.style = 0;
	wc.lpfnWndProc = WndProc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = hInstance;
	wc.hIcon = LoadIcon(GetModuleHandle(NULL), MAKEINTRESOURCE(IDI_ICON2));
	wc.hCursor = NULL;
	wc.hbrBackground = (HBRUSH)COLOR_WINDOW;
	wc.lpszMenuName = NULL;
	wc.lpszClassName = g_szClassName;
	wc.hIconSm = (HICON)LoadImage(GetModuleHandle(NULL), MAKEINTRESOURCE(IDI_ICON1), IMAGE_ICON, 16, 16, 0);
	if(!RegisterClassEx(&wc)) {
		MessageBox(NULL, "Class Registration Failed!", "Error!", MB_ICONEXCLAMATION | MB_OK);
		return 1;
	}

	hwnd = CreateWindowEx(NULL, g_szClassName, "Testing Window", WS_EX_TOPMOST | WS_POPUP, CW_USEDEFAULT, CW_USEDEFAULT,
							SCREEN_WIDTH, SCREEN_HEIGHT, NULL, NULL, hInstance, NULL);
	if(hwnd == NULL) {
		MessageBox(NULL, "Window Creation Failed!", "Error!", MB_ICONEXCLAMATION | MB_OK);
		return 2;
	}
	InitDX(hInstance, hwnd);
	ShowCursor(0);
	ShowWindow(hwnd, nCmdShow);
	UpdateWindow(hwnd);

	while(1) {
		while(PeekMessage(&Msg, NULL, 0, 0, PM_REMOVE) > 0) {
			TranslateMessage(&Msg);
			DispatchMessage(&Msg);
		}
		if(Msg.message == WM_QUIT) {
			break;
		}
		DetectInput();
		if(KeyState(DIK_LEFT)) {
			x -= speed;
		}
		if(KeyState(DIK_RIGHT)) {
			x += speed;
		}
		if(KeyState(DIK_UP)) {
			y -= speed;
		}
		if(KeyState(DIK_DOWN)) {
			y += speed;
		}
		if(KeyState(DIK_ESCAPE)) {
			PostMessage(hwnd, WM_DESTROY, 0, 0);
		}
		if(KeyState(DIK_Z)) {
			speed -= 0.5f;
		}
		if(KeyState(DIK_X)) {
			speed += 0.5f;
		}
		if(speed <= 0) {
			speed = 0.5f;
		}
		if(KeyState(DIK_Q)) {
			x = 200;
			y = 200;
			speed = 5.0f;
		}

		RenderRectangle(x, y, x + 20,y + 200);
		RenderTriangle(x, y);
	}
	CleanDX();
	return 0;
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
	switch(msg) {
		case WM_CLOSE:
			DestroyWindow(hwnd);
		break;

		case WM_DESTROY:
			PostQuitMessage(0);
		break;
		default:
			return DefWindowProc(hwnd, msg, wParam, lParam);
	}
	return 0;
}

void InitDX(HINSTANCE hInstance, HWND hwnd) {
	InitD3D(hwnd);
	InitDI(hInstance, hwnd);
}

void InitD3D(HWND hwnd) {
	d3d = Direct3DCreate9(D3D_SDK_VERSION);
	D3DPRESENT_PARAMETERS d3dpp;
	ZeroMemory(&d3dpp, sizeof(d3dpp));
	d3dpp.Windowed = FALSE;
	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
	d3dpp.hDeviceWindow = hwnd;
	d3dpp.BackBufferFormat = D3DFMT_X8R8G8B8;
	d3dpp.BackBufferWidth = SCREEN_WIDTH;
	d3dpp.BackBufferHeight = SCREEN_HEIGHT;
	d3d->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hwnd, D3DCREATE_SOFTWARE_VERTEXPROCESSING, &d3dpp, &d3ddev);
	//d3ddev->SetRenderState(D3DRS_ZWRITEENABLE, 0);
}

void InitDI(HINSTANCE hInstance, HWND hwnd) {
	DirectInput8Create(hInstance, DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&din, NULL);
	din->CreateDevice(GUID_SysKeyboard,	&keyb, NULL);
	keyb->SetDataFormat(&c_dfDIKeyboard);
	keyb->SetCooperativeLevel(hwnd, DISCL_NONEXCLUSIVE | DISCL_FOREGROUND);
}

void RenderTriangle(float x1, float y1) {
	CUSTOMVERTEX vertices[] = {
		{x1, y1, 1.0f, COL_WHITE},
		{x1 - 50, y1 - 100, 1.0f, COL_WHITE},
		{x1 + 50, y1 - 100, 1.0f, COL_WHITE}
	};
	
	d3ddev->CreateVertexBuffer(3 * sizeof(CUSTOMVERTEX), 0, CUSTOMFVF, D3DPOOL_DEFAULT, &v_buffer, NULL);

	VOID* pVoid = NULL;

	v_buffer->Lock(0, sizeof(vertices), (void**)&pVoid, 0);
	memcpy(pVoid, vertices, sizeof(vertices));
	v_buffer->Unlock();

	d3ddev->Clear(0, NULL, D3DCLEAR_TARGET, D3DCOLOR_XRGB(0, 0, 0), 1.0f, 0);
	d3ddev->BeginScene();
		d3ddev->SetStreamSource(0, v_buffer, 0, sizeof(CUSTOMVERTEX));
		d3ddev->SetFVF(CUSTOMFVF);
		d3ddev->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 1);
	d3ddev->EndScene();
	d3ddev->Present(NULL, NULL, NULL, NULL);
}
void RenderRectangle(float x1, float y1, float x2, float y2) {
	CUSTOMVERTEX vertices[] = {
		{x1, y1, 1.0f, COL_WHITE},
		{x2, y1, 1.0f, COL_WHITE},
		{x1, y2, 1.0f, COL_WHITE},
		{x2, y2, 1.0f, COL_WHITE}
	};

	d3ddev->CreateVertexBuffer(4 * sizeof(CUSTOMVERTEX), 0, CUSTOMFVF, D3DPOOL_DEFAULT, &v_buffer, NULL);

	VOID* pVoid = NULL;

	v_buffer->Lock(0, sizeof(vertices), (void**)&pVoid, 0);
	memcpy(pVoid, vertices, sizeof(vertices));
	v_buffer->Unlock();

	d3ddev->Clear(0, NULL, D3DCLEAR_TARGET, D3DCOLOR_XRGB(0, 0, 0), 1.0f, 0);
	d3ddev->BeginScene();

		d3ddev->SetStreamSource(0, v_buffer, 0,	sizeof(CUSTOMVERTEX));
		d3ddev->SetFVF(CUSTOMFVF);
		d3ddev->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);

	d3ddev->EndScene();
	d3ddev->Present(NULL, NULL, NULL, NULL);
}

void DetectInput(void) {
	keyb->Acquire();
	keyb->GetDeviceState(256, (LPVOID)keystate);
}

void CleanDX(void) {
	CleanD3D();
	CleanDI();
}

void CleanD3D(void) {
	v_buffer->Release();
	d3ddev->Release();
	d3d->Release();
}

void CleanDI(void) {
	keyb->Unacquire();
	din->Release();
}
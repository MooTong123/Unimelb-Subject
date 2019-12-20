#include <windows.h>
#include <commctrl.h>
#include <d3d9.h>
#include "resource.h"
#pragma comment (lib, "d3d9.lib")
const char g_szClassName[] = "myWindowClass";
HWND g_hToolbar = NULL;
int g_Destroy = 0;
LPDIRECT3D9 d3d;
LPDIRECT3DDEVICE9 d3ddev;
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
BOOL CALLBACK ToolDlgProc(HWND, UINT, WPARAM, LPARAM);
void InitD3D(HWND hwnd);
void RenderFrame(void);
void CleanD3D(void);

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {

	WNDCLASSEX wc;
	HWND hwnd;
	MSG Msg;

	wc.cbSize = sizeof(WNDCLASSEX);
	wc.style = 0;
	wc.lpfnWndProc = WndProc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = hInstance;
	wc.hIcon = LoadIcon(GetModuleHandle(NULL), MAKEINTRESOURCE(IDI_ICON1));
	wc.hCursor = LoadCursor(hInstance, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wc.lpszMenuName = MAKEINTRESOURCE(IDR_MENU1);
	wc.lpszClassName = g_szClassName;
	wc.hIconSm = (HICON)LoadImage(GetModuleHandle(NULL), MAKEINTRESOURCE(IDI_ICON2), IMAGE_ICON, 16, 16, 0);
	if(!RegisterClassEx(&wc)) {
		MessageBox(NULL, "Class Registration Failed!", "Error!", MB_ICONEXCLAMATION | MB_OK);
		return 1;
	}

	hwnd = CreateWindowEx(WS_EX_CLIENTEDGE, g_szClassName, "Testing Window", WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT,
							640, 480, NULL, NULL, hInstance, NULL);
	if(hwnd == NULL) {
		MessageBox(NULL, "Window Creation Failed!", "Error!", MB_ICONEXCLAMATION | MB_OK);
		return 2;
	}
	ShowWindow(hwnd, nCmdShow);
	UpdateWindow(hwnd);

	InitD3D(hwnd);

	while(1) {
		while(PeekMessage(&Msg, NULL, 0, 0, PM_REMOVE) > 0) {
			TranslateMessage(&Msg);
			DispatchMessage(&Msg);
		}
		if(Msg.message == WM_QUIT) {
			break;
		}
		RenderFrame();
	}
	CleanD3D();
	return 0;
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
	switch(msg) {
		break;
		case WM_COMMAND:
			switch(LOWORD(wParam)) {
				case IDM_EXIT:
					PostQuitMessage(0);
				break;
			}
		break;

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

void InitD3D(HWND hwnd) {
	d3d = Direct3DCreate9(D3D_SDK_VERSION);
	D3DPRESENT_PARAMETERS d3dpp;
	ZeroMemory(&d3dpp, sizeof(d3dpp));
	d3dpp.Windowed = TRUE;
	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
	d3dpp.hDeviceWindow = hwnd;
	d3d ->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hwnd, D3DCREATE_SOFTWARE_VERTEXPROCESSING, &d3dpp, &d3ddev);
}

void RenderFrame(void) {
	d3ddev ->Clear(0, NULL, D3DCLEAR_TARGET, D3DCOLOR_XRGB(0, 40, 100), 1.0f, 0);
	d3ddev ->BeginScene();
	
	d3ddev ->EndScene();
	d3ddev ->Present(NULL, NULL, NULL, NULL);
}

void CleanD3D(void) {
	d3ddev ->Release();
	d3d ->Release();
}
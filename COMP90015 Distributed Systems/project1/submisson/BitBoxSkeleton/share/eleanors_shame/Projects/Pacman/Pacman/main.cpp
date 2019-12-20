#include "main.h"


//D3D objects
LPDIRECT3D9			  d3dObject;
LPDIRECT3DDEVICE9	  d3dDevice;

//DInput objects
LPDIRECTINPUT8	      intInput;
LPDIRECTINPUTDEVICE8  devInputKB;

//global variables
const char			   g_szClassName[] = "myWindowClass";
BYTE				  keystate[256];
INT					  SCREEN_REFRESH   = 1;
//classes
class BALL {
	public:
		D3DXVECTOR3			pos;
		float				xstart, ystart;
		float				hspeed, vspeed;
		int					alarm[16];
		LPDIRECT3DTEXTURE9	texture;
		LPD3DXSPRITE		sprite;
		BALL(float x, float y, string path);
		~BALL(void);
		void				StepEvent(void);
		void				KeyEvent(void);
		void				DrawEvent(void);
}*balls[5];
class BAT {
	public:
		D3DXVECTOR3		   pos;
		float			   xstart, ystart, speed;
		LPDIRECT3DTEXTURE9 texture;
		LPD3DXSPRITE	   sprite;
		BAT(float x, float y, float spd, string path);
		~BAT(void);
		int				   align, alarm[16];
		void			   StepEvent(void);
		void			   KeyEvent(void);
		void			   DrawEvent(void);
}*bats[5];  
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
	INT			alarm[16] = {0};
	srand((UINT)time(0));
	WNDCLASSEX  wc;
	HWND	    hwnd;
	MSG			Msg;
	ZeroMemory(&alarm, sizeof(alarm));
	int i;
	
	wc.cbSize		 = sizeof(WNDCLASSEX);
	wc.style		 = 0;
	wc.lpfnWndProc   = WndProc;
	wc.cbClsExtra    = 0;
	wc.cbWndExtra    = 0;
	wc.hInstance     = hInstance;
	wc.hIcon		 = LoadIcon(hInstance, IDI_SHIELD);
	wc.hCursor		 = 0;
	wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wc.lpszMenuName  = 0;
	wc.lpszClassName = g_szClassName;
	wc.hIconSm		 = LoadIcon(hInstance, IDI_SHIELD);

	ShowCursor(0); 

	if(!RegisterClassEx(&wc)) {
		  MessageBox(0, "Class Registration Failed!", "Error!", MB_ICONEXCLAMATION | MB_OK);
		  return 0x01;
	}

	hwnd = CreateWindowEx(WS_EX_CLIENTEDGE, g_szClassName, "Pong+Classes", WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT,
								     SCREEN_WIDTH, SCREEN_HEIGHT, 0, 0, hInstance, 0);
	if(hwnd == 0) {
		  MessageBox(0, "Window Creation Failed!", "Error!", MB_ICONEXCLAMATION | MB_OK);
		  return 0x02;
	}

InitDX(hwnd, hInstance);

	ZeroMemEx(balls);
	ZeroMemEx(bats);

	ShowWindow(hwnd, SW_MAXIMIZE);
	UpdateWindow(hwnd);

	balls[0] = new BALL(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, sprBall);
	bats[0] = new BAT(20, SCREEN_HEIGHT / 2 - 15, 8.0f, sprBat);
	bats[1] = new BAT(580, SCREEN_HEIGHT / 2 - 15, 8.0f, sprBat);

	ZeroMemEx(Msg);
	DetectInput();

	while(Msg.message != WM_QUIT) {
		  if(PeekMessage(&Msg, 0, 0, 0, PM_REMOVE)) { TranslateMessage(&Msg);DispatchMessage(&Msg); }
		  else {
			    DetectInput();
				DoObjFunc(KeyEvent());
				if(KeyCheck(DIK_ESCAPE)) {
					PostQuitMessage(0);
					break;
				}
			    d3dBeginScene();
				for(i = 0; bats[i] != 0; i++) {
					bats[i] -> DrawEvent();
				}
				for(i = 0; balls[i] != 0; i++) {
					balls[i] -> DrawEvent();
				}
				d3dEndScene();

				for(i = 0; i < 16; i++) {
					alarm[i]--;
				}
		  }
	}
	CleanDX();
	return 0x0;
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

HRESULT InitD3D(HWND hwnd) {
	/* Initialises D3D objects, devices and settings */
	d3dObject = Direct3DCreate9(D3D_SDK_VERSION);

	D3DPRESENT_PARAMETERS presParams;
	ZeroMemory(&presParams, sizeof(presParams));

	presParams.Windowed					= TRUE;
	presParams.SwapEffect				= D3DSWAPEFFECT_DISCARD;
	presParams.BackBufferFormat			= D3DFMT_UNKNOWN;
	presParams.PresentationInterval		= D3DPRESENT_INTERVAL_ONE;

	return
	  d3dObject ->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hwnd, D3DCREATE_HARDWARE_VERTEXPROCESSING, &presParams, &d3dDevice);
}

VOID InitDInput(HWND hwnd, HINSTANCE hInstance) {
	/* Initialises DInput devices and settings */
	DirectInput8Create(hInstance, DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&intInput, 0);
	intInput ->CreateDevice(GUID_SysKeyboard, &devInputKB, 0);

	devInputKB ->SetDataFormat(&c_dfDIKeyboard);
	devInputKB ->SetCooperativeLevel(hwnd, DISCL_NONEXCLUSIVE | DISCL_FOREGROUND);
}

HRESULT InitDX(HWND hwnd, HINSTANCE hInstance) {
	/* Initialises all DirectX init functions previously defined */
	InitDInput(hwnd, hInstance);
	if(FAILED(InitD3D(hwnd))) {
		 return (HRESULT)1;
	}
	else {
		 return (HRESULT)0;
	}
}

VOID DetectInput(void) {
	/* Detects keyboard input and mouse movement */
	devInputKB ->Acquire();
	devInputKB ->GetDeviceState(256, (LPVOID)keystate);
}
VOID CleanD3D(void) {
	/* Releases D3D devices and objects */
	int i;
	for(i = 0; bats[i] != 0; i++) {
		delete bats[i];
	}
	for(i = 0; balls[i] != 0; i++) {
		delete balls[i];
	}
	d3dDevice -> Release();
	d3dObject -> Release();
}

VOID CleanDInput(void) {
	/* Releases DInput devices and objects */
	devInputKB ->Unacquire();
	intInput ->Release();
}

VOID CleanDX(void) {
	/* Shortcut to calling the above 2 functions */
	CleanD3D();
	CleanDInput();
}
int sign(int n) {
	return (n > 0) - (n < 0);
}
BALL::BALL(float x, float y, string path) {
	pos.x = x;
	pos.y = y;
	pos.z = 0;
	xstart = x;
	ystart = y;
	ZeroMemEx(texture);
	ZeroMemEx(sprite);
	if(FAILED(D3DXCreateSprite(d3dDevice, &sprite))) {
		MessageBox(0, "Sprite creation in instance of BALL failed.", "Error", MB_ICONERROR | MB_OK);
	}
	if(FAILED(D3DXCreateTextureFromFile(d3dDevice, path, &texture))) {
		MessageBox(0, "Sprite creation in instance of BALL failed.", "Error", MB_ICONERROR | MB_OK);
	}
}
void BALL::KeyEvent(void) {
	if(KeyCheck(DIK_R)) {
		pos.x = xstart;
		pos.y = ystart;
	}
}
void BALL::StepEvent(void) {
	int i;
	
	pos.x += hspeed;
	pos.y += vspeed;

	for(i = 15; i > -1; i--) {
		alarm[i]--;
	}
}
void BALL::DrawEvent(void) {
	sprite -> Begin(0);
	sprite -> Draw(texture, 0, 0, &pos, cWhite);
	sprite -> End();
}
BALL::~BALL(void) {
	sprite -> Release();
	texture -> Release();
}
BAT::BAT(float x, float y, float spd, string path) {
	pos.x = x;
	pos.y = y;
	pos.z = 0;
	xstart = x;
	ystart = y;
	speed = spd;
	align = abs(sign((int)x) - (SCREEN_WIDTH / 2));
	if(FAILED(D3DXCreateSprite(d3dDevice, &sprite))) {
		MessageBox(0, "Sprite creation in instance of BAT failed.", "Error", MB_ICONERROR | MB_OK);
	}
	if(FAILED(D3DXCreateTextureFromFile(d3dDevice, path, &texture))) {
		MessageBox(0, "Sprite creation in instance of BAT failed.", "Error", MB_ICONERROR | MB_OK);
	}
}
void BAT::KeyEvent(void) {
	if(KeyCheck(DIK_W)) pos.y -= speed;
	if(KeyCheck(DIK_S)) pos.y += speed;
}
void BAT::StepEvent(void) {
	int i;

	for(i = 15; i > -1; i--) {
		alarm[i]--;
	}
}
void BAT::DrawEvent(void) {
	sprite -> Begin(D3DXSPRITE_ALPHABLEND);
	sprite -> Draw(texture, 0, 0, &pos, cWhite);
 	sprite -> End();
}
BAT::~BAT(void) {
	sprite -> Release();
	texture -> Release();
}
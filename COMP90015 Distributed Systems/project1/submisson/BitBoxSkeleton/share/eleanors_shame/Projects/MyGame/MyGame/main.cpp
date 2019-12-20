#include "main.h"

//global variables
const char			   g_szClassName[] = "myWindowClass";

LPDIRECT3D9			  d3dObject;
LPDIRECT3DDEVICE9	  d3dDevice;

LPDIRECTINPUT8	      intInput;
LPDIRECTINPUTDEVICE8  devInput;
BYTE				  keystate[256];
INT					  alarm[16];
INT					  SCREEN_REFRESH = 1;
INT					  CollisionGrid[SCREEN_WIDTH / 16][SCREEN_HEIGHT / 16];

//function prototypes
LRESULT CALLBACK	  WndProc(HWND, UINT, WPARAM, LPARAM);
HRESULT			      InitDX(HWND, HINSTANCE);
HRESULT				  InitD3D(HWND);
VOID				  InitDInput(HWND, HINSTANCE);
VOID				  CleanDX(void);
VOID				  CleanD3D(void);
VOID				  CleanDInput(void);
VOID				  DetectInput(void);
class block {
	public:
		  D3DXVECTOR3				     blockpos;
		  LPD3DXSPRITE					 pSprite;
		  LPDIRECT3DTEXTURE9			 pTexture;
		  LPCSTR						 path;
		  int							 canMove;
		  int							 type;
		  float							 xprevious;
		  float							 yprevious;

		  void				LoadSprite(void);
		  void				DrawSprite(void);
};
class block01 : public block {
	public:
		block01::block01(float, float, LPCSTR);
		block01::~block01(void);
		int type;
};
INT					  CheckCollisionGrid(block*);
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
	WNDCLASSEX  wc;
	HWND	    hwnd;
	MSG			Msg;
	INT			BlockCount = 0;
	INT			Timer = 0;
	INT			i;
	block*      Blocks[100];
	ZeroMemory(&alarm, sizeof(alarm));

	wc.cbSize		 = sizeof(WNDCLASSEX);
	wc.style		 = 0;
	wc.lpfnWndProc   = WndProc;
	wc.cbClsExtra    = 0;
	wc.cbWndExtra    = 0;
	wc.hInstance     = hInstance;
	wc.hIcon		 = 0;
	wc.hCursor		 = 0;
	wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wc.lpszMenuName  = 0;
	wc.lpszClassName = g_szClassName;
	wc.hIconSm		 = 0;
	if(!RegisterClassEx(&wc)) {
		  MessageBox(0, "Class Registration Failed!", "Error!", MB_ICONEXCLAMATION | MB_OK);
		  return 0x1;
	}
	//ShowCursor(0);

	hwnd = CreateWindowEx(WS_EX_CLIENTEDGE, g_szClassName, "Testing Window", WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT,
								     SCREEN_WIDTH, SCREEN_HEIGHT, 0, 0, hInstance, 0);
	if(hwnd == 0) {
		  MessageBox(0, "Window Creation Failed!", "Error!", MB_ICONEXCLAMATION | MB_OK);
		  return 0x2;
	}

	ShowWindow(hwnd, nCmdShow);
	UpdateWindow(hwnd);



InitDX(hwnd, hInstance);


	block01 MyBlock(16.0f, 0.0f, BLOCK01);
	Blocks[0] = &MyBlock;
	BlockCount++;

	d3dDevice ->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);

	ZeroMemory(&Msg, sizeof(Msg));
	while(Msg.message != WM_QUIT) {
		  if(PeekMessage(&Msg, 0, 0, 0, PM_REMOVE)) {
			    TranslateMessage(&Msg);
			    DispatchMessage(&Msg);
		  }
		  else {
			    if(CheckCollisionGrid(Blocks[CURRENTBLOCK])) {
					MessageBox(hwnd, "yay", "test", MB_OK);
				}
			    DetectInput();
				if(KeyCheck(DIK_ESCAPE)) {
					PostQuitMessage(0);
				}
				if(KeyCheck(DIK_LEFT) && AlarmCheck(1)) {
					Blocks[CURRENTBLOCK] ->blockpos.x -= 16;
					alarm[1] = 15;
				}
				if(KeyCheck(DIK_RIGHT) && AlarmCheck(1)) {
					Blocks[CURRENTBLOCK] ->blockpos.x += 16;
					alarm[1] = 15;
				}
				if(KeyCheck(DIK_DOWN) && AlarmCheck(2)) {
					Blocks[CURRENTBLOCK] ->blockpos.y += 16;
					alarm[2] = 15;
				}
				if(KeyCheck(DIK_UP) && AlarmCheck(3)) {
					Blocks[CURRENTBLOCK] ->blockpos.y = SCREEN_HEIGHT - 72;
					alarm[3] = 15;
					StopBlockMove();
				}
			    if(AlarmCheck(0)) {
				     if(Blocks[CURRENTBLOCK] ->canMove == 1) {
					   Blocks[CURRENTBLOCK] ->blockpos.y += 16;
				     }
				     if(Blocks[CURRENTBLOCK] ->blockpos.y >= SCREEN_HEIGHT - 72) {
					      StopBlockMove();
						  
						  switch(Blocks[CURRENTBLOCK] ->type) {
							  case 0x01:
					CollisionGrid[(int)(Blocks[CURRENTBLOCK] ->blockpos.x / 16)][(int)(Blocks[CURRENTBLOCK] ->blockpos.y / 16)] = 1;
					CollisionGrid[(int)(Blocks[CURRENTBLOCK] ->blockpos.x / 16) + 1][(int)(Blocks[CURRENTBLOCK] ->blockpos.y / 16)] = 1;
					CollisionGrid[(int)(Blocks[CURRENTBLOCK] ->blockpos.x / 16)][(int)(Blocks[CURRENTBLOCK] ->blockpos.y / 16) + 1] = 1;
					CollisionGrid[(int)(Blocks[CURRENTBLOCK] ->blockpos.x / 16) + 1][(int)(Blocks[CURRENTBLOCK] ->blockpos.y / 16) + 1] = 1;
								  break;
						  }
					      Blocks[CURRENTBLOCK] ->blockpos.y = SCREEN_HEIGHT - 72;
					      Blocks[BlockCount] = new block01(SCREEN_WIDTH / 2 - 16, 0.0f, BLOCK01);
					      BlockCount++;
				     }
				     alarm[0] = 30;
			    }
				
			    d3dBeginScene();
			     for(i = 0; i != BlockCount; i++) {
				      Blocks[i] ->DrawSprite();
			     }
			    d3dEndScene();
				
		  }
		  for(i = 0; i < 16; i++) {
			  alarm[i]--;
			  if(alarm[i] <= 0) {
				  alarm[i] = 0;
			  }
		  }
		  
	}
	CleanDX();
	return 0x0;
}

block01::block01(float xx, float yy, LPCSTR text) {
	blockpos.x = xx;
	blockpos.y = yy;
	blockpos.z = 1.0f;
	canMove = 1;
	type = 0x01;
	path = text;
	LoadSprite();
}
block01::~block01() {
	pTexture ->Release();
	pSprite  ->Release();
}
void block::LoadSprite(void) {
	if(FAILED(D3DXCreateTextureFromFile(d3dDevice, path, &pTexture))) {
		 MessageBox(0, "Error loading texture.", "Error", MB_ICONHAND | MB_OK);
	}
	if(FAILED(D3DXCreateSprite(d3dDevice, &pSprite))) {
		 MessageBox(0, "Error loading sprite.", "Error", MB_ICONHAND | MB_OK);
	}
}
void block::DrawSprite(void) {
	pSprite ->Begin(D3DXSPRITE_ALPHABLEND);
	 pSprite ->Draw(pTexture, 0, 0, &blockpos, 0xffffffff);
	pSprite ->End();
	xprevious = blockpos.x;
	yprevious = blockpos.y;
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
	switch(msg) {
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

HRESULT InitD3D(HWND hwnd) {
	d3dObject = Direct3DCreate9(D3D_SDK_VERSION);

	D3DPRESENT_PARAMETERS presParams;
	ZeroMemory(&presParams, sizeof(presParams));

	presParams.Windowed =			      TRUE;
	presParams.SwapEffect =			    D3DSWAPEFFECT_DISCARD;
	presParams.BackBufferFormat =	    D3DFMT_UNKNOWN;
	presParams.PresentationInterval =	D3DPRESENT_INTERVAL_ONE;

	return
	  d3dObject ->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hwnd, D3DCREATE_HARDWARE_VERTEXPROCESSING, &presParams, &d3dDevice);
}

VOID InitDInput(HWND hwnd, HINSTANCE hInstance) {
	DirectInput8Create(hInstance, DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&intInput, 0);
	intInput ->CreateDevice(GUID_SysKeyboard, &devInput, 0);

	devInput ->SetDataFormat(&c_dfDIKeyboard);
	devInput ->SetCooperativeLevel(hwnd, DISCL_NONEXCLUSIVE | DISCL_FOREGROUND);
}

HRESULT InitDX(HWND hwnd, HINSTANCE hInstance) {
	InitDInput(hwnd, hInstance);
	if(FAILED(InitD3D(hwnd))) {
		 return (HRESULT)1;
	}
	else {
		 return (HRESULT)0;
	}
}

VOID DetectInput(void) {
	devInput ->Acquire();
	devInput ->GetDeviceState(256, (LPVOID)keystate);
}

INT CheckCollisionGrid(block* Block) {
	INT n = 0;
	switch(Block ->type) {
		case 0x01:
			if( (CollisionGrid[(int)(Block ->blockpos.x / 16)][(int)(Block ->blockpos.y / 16)] == 1) ||
				(CollisionGrid[(int)(Block ->blockpos.x / 16) + 1][(int)(Block ->blockpos.y / 16)] == 1) ||
				(CollisionGrid[(int)(Block ->blockpos.x / 16) + 1][(int)(Block ->blockpos.y / 16) + 1] == 1) ||
				(CollisionGrid[(int)(Block ->blockpos.x / 16)][(int)(Block ->blockpos.y / 16) + 1] == 1)
			  ) {
				  n = 1;
			}
			else {
				n = 0;
			}
			break;
	}
	return n;
}
VOID CleanD3D(void) {
	d3dDevice ->Release();
	d3dObject ->Release();
}

VOID CleanDInput(void) {
	devInput ->Unacquire(); 
	intInput ->Release();
}

VOID CleanDX(void) {
	CleanD3D();
	CleanDInput();
}
	
#include "pongout.h"


//D3D objects
LPDIRECT3D9			  d3dObject;
LPDIRECT3DDEVICE9	  d3dDevice;
LPD3DXFONT			  fntPong;

//DInput objects
LPDIRECTINPUT8	      intInput;
LPDIRECTINPUTDEVICE8  devInputKB;

//global variables
const char			   g_szClassName[] = "myWindowClass";
BYTE				  keystate[256];
INT					  alarm[16]        = {0};
INT					  SCREEN_REFRESH   = 1;
INT					  __PC			   = -1;
INT					  BrickCount	   = 0;
char*				  BRICKSPRS[1];
char				  BRICK01[]		   = BRICK01SPR;
FLOAT				  ENEMY_DIFFICULTY = 1.2f;

class BRICK {
	public:
		D3DXVECTOR3		   pos;
		LPDIRECT3DTEXTURE9 myTex;
		LPD3DXSPRITE	   mySpr;
		RECT			   cllRect;
		RECT*			   cllRectPtr;
		void			   DrawSprite(void);
		void			   Cleanup(void);
		BRICK(float x, float y, LPCSTR pth);
}*bricks[MAX_BRICK_COUNT];		   
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
	BRICKSPRS[0] = BRICK01;
	srand((UINT)time(0));
	MessageBox(0, PONGOPONGO_VERSION_NAME, "Version", MB_ICONINFORMATION | MB_OK);
	WNDCLASSEX  wc;
	HWND	    hwnd;
	MSG			Msg;
	int			i = 0, ii = 0, select = 0;
	float		KEYBOARD_SENSITIVITY = 7.0f;
	ZeroMemory(&alarm, sizeof(alarm));
	
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

	hwnd = CreateWindowExA(WS_EX_CLIENTEDGE, g_szClassName, "Pongout	", WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT,
								     SCREEN_WIDTH, SCREEN_HEIGHT, 0, 0, hInstance, 0);
	if(hwnd == 0) {
		  MessageBox(0, "Window Creation Failed!", "Error!", MB_ICONEXCLAMATION | MB_OK);
		  return 0x02;
	}


InitDX(hwnd, hInstance);


	D3DXCreateFont(d3dDevice, 64, 0, FW_BOLD, 0, 0, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE,
					TEXT("System"), &fntPong);
	LPDIRECT3DTEXTURE9 texBat, texBall, texCourt;
	LPD3DXSPRITE	   sprBat, sprBall, sprCourt;
	D3DXVECTOR3		   posPlayer(50.0f, (float)SCREEN_HEIGHT / 2.0f - 36.0f, 0.0f),
					   posBall((float)SCREEN_WIDTH / 2.0f - 5.5f, (float)SCREEN_HEIGHT / 2.0f - 16.0f, 0.0f),
					   posEnemy((float)SCREEN_WIDTH - 86.0f, (float)SCREEN_HEIGHT / 2.0f - 36.0f, 0.0f),
					   posCourt((float)SCREEN_WIDTH / 2 - 6, 0, 0);
	float			   posEnemyYstart = posEnemy.y, posBallYstart = posBall.y, posBallXstart = posBall.x;
	RECT			   rectPlayer, rectEnemy, rectBall, dummy;
	float			   ballHSpeed = 0, ballVSpeed = 0;
	int				   pScore = 0, eScore = 0;
	char			   SCORETEXT[20];
	sprintf(SCORETEXT, "%d    %d", pScore, eScore);
	ZeroMemEx(texBat);
	ZeroMemEx(texBall);
	ZeroMemEx(texCourt);
	ZeroMemEx(sprBat);
	ZeroMemEx(sprBall);
	ZeroMemEx(sprCourt);
	ZeroMemEx(bricks);

	if(FAILED(D3DXCreateTextureFromFile(d3dDevice, BATSPR, &texBat))) {
		MessageBox(hwnd, "Failed loading texture: texBat.", "ERROR", MB_ICONERROR | MB_OK);
		ExCleanDX();
		return 0x03;
	}
	if(FAILED(D3DXCreateTextureFromFile(d3dDevice, BALLSPR, &texBall))) {
		MessageBox(hwnd, "Failed loading texture: texBall.", "ERROR", MB_ICONERROR | MB_OK);
		ExCleanDX();
		return 0x04;
	}
	if(FAILED(D3DXCreateTextureFromFile(d3dDevice, COURTSPR, &texCourt))) {
		MessageBox(hwnd, "Failed loading texture: texCourt.", "ERROR", MB_ICONERROR | MB_OK);
		ExCleanDX();
		return 0x05;
	}


	if(FAILED(D3DXCreateSprite(d3dDevice, &sprBat))) {
		MessageBox(hwnd, "Failed creating sprite: sprBat.", "ERROR", MB_ICONERROR | MB_OK);
		ExCleanDX();
		return 0x06;
	}
	if(FAILED(D3DXCreateSprite(d3dDevice, &sprBall))) {
		MessageBox(hwnd, "Failed creating sprite: sprBall.", "ERROR", MB_ICONERROR | MB_OK);
		ExCleanDX();
		return 0x07;
	}
	if(FAILED(D3DXCreateSprite(d3dDevice, &sprCourt))) {
		MessageBox(hwnd, "Failed creating sprite: sprCourt.", "ERROR", MB_ICONERROR | MB_OK);
		ExCleanDX();
		return 0x08;
	}

	if(__argc > 1) {
		KEYBOARD_SENSITIVITY = (float)atoi(__argv[1]);
		if(__argc > 2) {
			ENEMY_DIFFICULTY  = (float)atoi(__argv[2]);
		}
	}

	d3dDevice ->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	ShowWindow(hwnd, SW_MAXIMIZE);
	UpdateWindow(hwnd);
	d3dBeginScene();
		ShowText(cDkGreen, "_---Pongout---_", SCREEN_WIDTH / 2 - 200, SCREEN_WIDTH / 2 + 286, 176, 304);
		ShowText(cWhite, "This game is not sold", SCREEN_WIDTH / 2 - 290, SCREEN_WIDTH / 2 + 286, 240, 368);
	d3dEndScene();
	Sleep(2000);

	game:
	for(i = 0; i < MAX_BRICK_COUNT; i++) {
		delete bricks[i];
	}
	ZeroMemEx(bricks);
	BrickCount = 0;
	KEYBOARD_SENSITIVITY = LoadLevel();
	pScore = 0;
	eScore = 0;
	__PC = -1;

	ZeroMemEx(Msg);
	DetectInput();

	while(__PC < 0) {
		if(PeekMessage(&Msg, 0, 0, 0, PM_REMOVE)) { TranslateMessage(&Msg);DispatchMessage(&Msg); }
		else {
			DetectInput();
			
			if(KeyCheck(DIK_ESCAPE)) {
				PostQuitMessage(0);
			}
			if(KeyCheck(DIK_S) && AlarmCheck(2)) {
				select += 1;
				if(select > 2) {
					select = 0;
				}
				alarm[2] = 10;
			}
			if(!KeyCheck(DIK_S)) {
				alarm[2] = 0;
			}
			if(KeyCheck(DIK_W) && AlarmCheck(3)) {
				select -= 1;
				if(select < 0) {
					select = 2;
				}
				alarm[3] = 10;
			}
			if(!KeyCheck(DIK_W)) {
				alarm[3] = 0;
			}

			if(KeyCheck(DIK_SPACE)) {
				switch(select) {
					case 0:
						__PC = 1;
						break;
					case 1:
						__PC = 2;
						break;
					default:
						ExCleanDX();
						return 0;
				}
			}
			d3dBeginScene();
				ShowText(((select == 0) ? cLime : cWhite), "Begin 1P Game", SCREEN_WIDTH / 2 - 220, SCREEN_WIDTH / 2 + 556, 112, 240);
				ShowText(((select == 1) ? cLime : cWhite), "Begin 2P Game", SCREEN_WIDTH / 2 - 220, SCREEN_WIDTH / 2 + 556, 176, 304);
				ShowText(((select == 2) ? cLime : cWhite), "Exit Game", SCREEN_WIDTH / 2 - 220, SCREEN_WIDTH / 2 + 356, 240, 368);
			d3dEndScene();
			for(i = 0; i < 16; i++) {
				alarm[i]--;
			}
		}
	}
	for(i = 0; i < 16; i++) {
		alarm[i] = 0;
	}
	alarm[0] = 20;
	while(Msg.message != WM_QUIT) {
		  if(PeekMessage(&Msg, 0, 0, 0, PM_REMOVE)) { TranslateMessage(&Msg);DispatchMessage(&Msg); }
		  else {
			    DetectInput();
				if(KeyCheck(DIK_ESCAPE)) {
					PostQuitMessage(0);
					break;
				}
				if(KeyCheck(DIK_R)) {
					if(KeyCheck(DIK_G)) {
						goto game;
						break;
					}
					ballHSpeed = 0;
					ballVSpeed = 0;
					posBall.x = posBallXstart;
					posBall.y = posBallYstart;
					alarm[0] = 20;
				}
				if(KeyCheck(DIK_W)) {
					posPlayer.y -= KEYBOARD_SENSITIVITY;
				}
				if(KeyCheck(DIK_S)) {
					posPlayer.y += KEYBOARD_SENSITIVITY;
				}
				
				if(posPlayer.y <= 0) {
					posPlayer.y = 0;
				}
				if(posPlayer.y >= SCREEN_HEIGHT - 106) {
					posPlayer.y = SCREEN_HEIGHT - 106;
				}

				if(alarm[0] == 0) {
					SetDirection(&ballHSpeed, &ballVSpeed);
					alarm[0] = -2;
				}

				if(posBall.y <= 8 || posBall.y >= SCREEN_HEIGHT - 56) {
					ballVSpeed = -ballVSpeed;
					if(rand() % 2 == 0) {
						PlaySound(HITSND01, 0, SND_CUSTOM);
					}
					else {
						PlaySound(HITSND02, 0, SND_CUSTOM);
					}
				}
				if(posBall.x <= 10 + (abs(ballHSpeed) - 4) || posBall.x >= SCREEN_WIDTH - (54 - (abs(ballHSpeed) + 4))) {
					if(sign((int)ballHSpeed) == 1) {
						pScore++;
						SOUND01();
						SOUND02();
						SOUND01();
					}
					else {
						eScore++;
						SOUND02();
						SOUND01();
						SOUND02();
					}
					ballHSpeed = 0;
					ballVSpeed = 0;
					posBall.x = SCREEN_WIDTH / 2;
					posBall.y = SCREEN_HEIGHT / 2;
					alarm[0] = 20;
				}
				if(__PC == 1) {
					if(posBall.x >= SCREEN_WIDTH / 2 && sign((int)ballHSpeed) == 1) {
						if(posBall.y < posEnemy.y + 28) {
							posEnemy.y -= ((posEnemy.y + 28 - posBall.y) > ENEMY_MAXSPEED) ? ENEMY_MAXSPEED : mean(ballHSpeed, ballVSpeed) - 1.5f;
						}
						else if(posBall.y > posEnemy.y + 28) {
							posEnemy.y += ((posBall.y - posEnemy.y + 28) > ENEMY_MAXSPEED) ? ENEMY_MAXSPEED : mean(ballHSpeed, ballVSpeed) - 1.5f;
						}
					}
					else if(posEnemy.y > posEnemyYstart) {
						posEnemy.y -= ((posEnemy.y - posEnemyYstart) < ENEMY_MAXSPEED) ? 1.0f : ENEMY_MAXSPEED;
					}
					else if(posEnemy.y < posEnemyYstart) {
						posEnemy.y += ((posEnemyYstart - posEnemy.y) < ENEMY_MAXSPEED) ? 1.0f : ENEMY_MAXSPEED;
					}
				}
				else if(__PC == 2) {
					if(KeyCheck(DIK_UP)) {
						posEnemy.y -= KEYBOARD_SENSITIVITY;
					}
					if(KeyCheck(DIK_DOWN)) {
						posEnemy.y += KEYBOARD_SENSITIVITY;
					}
				}
				else {
					return 0x09;
				}
				if(posEnemy.y <= 0) {
					posEnemy.y = 0;
				}
				if(posEnemy.y >= SCREEN_HEIGHT - 106) {
					posEnemy.y = SCREEN_HEIGHT - 106;
				}
				rectPlayer.left     = (LONG)posPlayer.x;
				rectPlayer.top	    = (LONG)posPlayer.y - 4;
				rectPlayer.bottom   = (LONG)posPlayer.y + 60;
				rectPlayer.right    = (LONG)posPlayer.x + 18;

				rectEnemy.left		= (LONG)posEnemy.x;
				rectEnemy.right		= (LONG)posEnemy.x + 18;
				rectEnemy.top		= (LONG)posEnemy.y - 4;
				rectEnemy.bottom	= (LONG)posEnemy.y + 60;

				rectBall.left		= (LONG)posBall.x;
				rectBall.right		= (LONG)posBall.x + 8;
				rectBall.top		= (LONG)posBall.y;
				rectBall.bottom		= (LONG)posBall.y + 8;

				if(IntersectRect(&dummy, &rectBall, &rectPlayer) && sign((int)ballHSpeed) == -1) {
					if(posBall.y >= posPlayer.y + 56 || posBall.y <= posPlayer.y - 8) {
						ballVSpeed = -ballVSpeed;
					}
					ballHSpeed = -ballHSpeed;
					ballHSpeed += INCREASE_SPEED - ((max(posBall.y, posPlayer.y + 28) - min(posBall.y, posPlayer.y + 28)) / WEARDOWN);
					if(sign((int)ballVSpeed) == 1) {
						ballVSpeed += INCREASE_SPEED - ((max(posBall.y, posPlayer.y + 28) - min(posBall.y, posPlayer.y + 28)) / (WEARDOWN / 2));
					}
					else {
						ballVSpeed -= INCREASE_SPEED - ((max(posBall.y, posPlayer.y + 28) - min(posBall.y, posPlayer.y + 28)) / (WEARDOWN / 2));
					}
					if(alarm[1] <= 0) {
						PlaySound(HITSND01, 0, SND_CUSTOM);
						alarm[1] = 10;
					}
				}
				if(IntersectRect(&dummy, &rectEnemy, &rectBall) && sign((int)ballHSpeed) == 1) {
					if(posBall.y >= posEnemy.y + 56 || posBall.y <= posEnemy.y - 8) {
						ballVSpeed = -ballVSpeed;
					}
					ballHSpeed = -ballHSpeed;
					ballHSpeed -= INCREASE_SPEED - ((max(posBall.y, posEnemy.y + 28) - min(posBall.y, posEnemy.y + 28)) / WEARDOWN);
					if(sign((int)ballVSpeed) == 1) {
						ballVSpeed += INCREASE_SPEED - ((max(posBall.y, posEnemy.y + 28) - min(posBall.y, posEnemy.y + 28)) / (WEARDOWN / 2));
					}
					else {
						ballVSpeed -= INCREASE_SPEED - ((max(posBall.y, posEnemy.y + 28) - min(posBall.y, posEnemy.y + 28)) / (WEARDOWN / 2));
					}
					if(alarm[1] <= 0) {
						PlaySound(HITSND02, 0, SND_CUSTOM);
						alarm[1] = 10;
					}
				}
				for(i = 0; i < BrickCount; i++) {
					if(IntersectRect(&dummy, bricks[i] ->cllRectPtr, &rectBall)) {
						if(posBall.y >= bricks[i] ->pos.y + 30 + ballSpeed || posBall.y <= bricks[i] ->pos.y - ballSpeed) {
							ballVSpeed = -ballVSpeed;
							posBall.y += ballVSpeed;
						}
						ballHSpeed = -ballHSpeed;
						posBall.x += ballHSpeed;
						
						if(rand() % 2) {
							PlaySound(HITSND01, 0, SND_CUSTOM);
						}
						else {
							PlaySound(HITSND02, 0, SND_CUSTOM);
						}
						delete bricks[i];
						for(ii = i; ii < BrickCount; ii++) {
							bricks[ii] = bricks[ii + 1];
						}
						BrickCount--;
					}
				}

				posBall.x += ballHSpeed;
				posBall.y += ballVSpeed;

				if(pScore >= SCORE_LIMIT && eScore < pScore - 1) {
					sprintf(SCORETEXT, "%d    %d", pScore, eScore);
					d3dBeginScene();
						sprCourt ->Begin(D3DXSPRITE_ALPHABLEND);
							sprCourt ->Draw(texCourt, 0, 0, &posCourt, cWhite);
						sprCourt ->End();
						sprBall ->Begin(D3DXSPRITE_ALPHABLEND);
							sprBall ->Draw(texBall, 0, 0, &posBall, cWhite);
						sprBall ->End();
						sprBat ->Begin(D3DXSPRITE_ALPHABLEND);
							sprBat ->Draw(texBat, 0, 0, &posPlayer, cWhite);
							sprBat ->Draw(texBat, 0, 0, &posEnemy, cWhite);
						sprBat ->End();
						ShowText(cLime, SCORETEXT);
					d3dEndScene();
					SOUND03();
					Sleep(1000);
					d3dDevice ->BeginScene();
						if(__PC == 1) {
							ShowText(cDkGreen, "Player Wins!", SCREEN_WIDTH / 2 - 180, SCREEN_WIDTH / 2 + 256, 176, 304);
						}
						else {
							ShowText(cDkGreen, "Player 1 Wins!", SCREEN_WIDTH / 2 - 180, SCREEN_WIDTH / 2 + 256, 176, 304);
						}
					d3dEndScene();
					Sleep(2000);
					goto game;
					break;
				}
				else if(eScore >= SCORE_LIMIT && pScore < eScore - 1) {
					sprintf(SCORETEXT, "%d    %d", pScore, eScore);
					d3dBeginScene();
						sprCourt ->Begin(0);
							sprCourt ->Draw(texCourt, 0, 0, &posCourt, cWhite);
						sprCourt ->End();
						sprBat ->Begin(0);
							sprBat ->Draw(texBat, 0, 0, &posPlayer, cWhite);
							sprBat ->Draw(texBat, 0, 0, &posEnemy, cWhite);
						sprBat ->End();

						sprBall ->Begin(0);
							sprBall ->Draw(texBall, 0, 0, &posBall, cWhite);
						sprBall ->End();
						ShowText(cLime, SCORETEXT);
					d3dEndScene();
					SOUND04();
					Sleep(1000);
					d3dDevice ->BeginScene();
						if(__PC == 1) {
							ShowText(cDkGreen, "Comp. Wins!", SCREEN_WIDTH / 2 - 180, SCREEN_WIDTH / 2 + 256, 176, 304);
						}
						else {
							ShowText(cDkGreen, "Player 2 Wins!", SCREEN_WIDTH / 2 - 180, SCREEN_WIDTH / 2 + 256, 176, 304);
						}
					d3dEndScene();
					Sleep(2000);
					goto game;
					break;
				}
				sprintf(SCORETEXT, "%d    %d", pScore, eScore);
			    d3dBeginScene();
					sprCourt ->Begin(0);
						sprCourt ->Draw(texCourt, 0, 0, &posCourt, cWhite);
					sprCourt ->End();
					ShowText(cLime, SCORETEXT);
					sprBall ->Begin(0);
						sprBall ->Draw(texBall, 0, 0, &posBall, cWhite);
					sprBall ->End();
					sprBat ->Begin(0);
						sprBat ->Draw(texBat, 0, 0, &posPlayer, cWhite);
						sprBat ->Draw(texBat, 0, 0, &posEnemy, cWhite);
					sprBat ->End();
					for(i = 0; i != BrickCount; i++) {
						bricks[i] ->DrawSprite();
					}
			    d3dEndScene();

				for(i = 0; i < 16; i++) {
					alarm[i]--;
				}
		  }
	}
	ExCleanDX();
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


BRICK::BRICK(float x, float y, LPCSTR pth) {
	pos.x = x;
	pos.y = y;
	pos.z = 0.0f;
	ZeroMemEx(myTex);
	ZeroMemEx(mySpr);

	cllRect.left = (long)x;
	cllRect.top = (long)y;
	cllRect.right = (long)x + 8;
	cllRect.bottom = (long)y + 30;
	cllRectPtr = &cllRect;

	if(FAILED(D3DXCreateSprite(d3dDevice, &mySpr))) {
		MessageBox(0, "Sprite creation in instance of BRICK failed.", "Error", MB_ICONERROR | MB_OK);
	}
	if(FAILED(D3DXCreateTextureFromFile(d3dDevice, pth, &myTex))) {
		MessageBox(0, "Texture creation in instance of BRICK failed.", "Error", MB_ICONERROR | MB_OK);
	}
}
void BRICK::DrawSprite(void) {
	mySpr ->Begin(D3DXSPRITE_ALPHABLEND);
		mySpr ->Draw(myTex, 0, 0, &pos, cWhite);
	mySpr ->End();
}
void BRICK::Cleanup(void) {
	mySpr ->Release();
	myTex ->Release();
}
INT sign(int n) {
	/* Returns the sign of n */
	if(n > 0) {
		return 1;
	}
	else if(n == 0) {
		return 0;
	}
	else if(n < 0) {
		return -1;
	}
	return -2;
}
VOID SetDirection(float* HV, float* VV) {	
	/* Sets the values *HV, *VV to a rand()om direction chosen from preset values */
	switch(rand() % 8) {
		case 0:
			*HV = -4;
			*VV = -2;
			break;
		case 1:
			*HV = -4;
			*VV = -4;
			break;
		case 2:
			*HV = -4;
			*VV = 2;
			break;
		case 3:
			*HV = 4;
			*VV = -4;
			break;
		case 4:
			*HV = 4;
			*VV = -2;
			break;
		case 5:
			*HV = 4;
			*VV = 4;
			break;
		case 6:
			*HV = 4;
			*VV = 2;
			break;
		case 7:
			*HV = -4;
			*VV = 4;
			break;
		default:
			*HV = -4;
	}
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

VOID ShowText(D3DCOLOR colour, char* text, LONG left, LONG right, LONG top, LONG bottom) {
	/* Draws text within a certain area */
	RECT rct;
	rct.left	= left;
	rct.right	= right;
	rct.top		= top;
	rct.bottom	= bottom;

	fntPong ->DrawText(0, text, -1, &rct, 0, colour);
}
VOID ShowText(D3DCOLOR colour, char* text, LONG left, LONG right) {
	/* Draws text at the top of the screen within a given left/right value */
	/* Note left and right coordinates are reversed (oops) */
	RECT rct;
	rct.left	= SCREEN_WIDTH / 2 - right;
	rct.right	= SCREEN_WIDTH / 2 + left;
	rct.top		= 16;
	rct.bottom	= 80;

	fntPong ->DrawText(0, text, -1, &rct, 0, colour);
}
VOID ShowText(D3DCOLOR colour, char* text) {
	/* Draws text within a preset rectangle */
	RECT rct;
	rct.left	= SCREEN_WIDTH / 2 - 67;
	rct.right	= SCREEN_WIDTH / 2 + 64;
	rct.top		= 16;
	rct.bottom	= 80;

	fntPong ->DrawText(0, text, -1, &rct, 0, colour);
}
VOID DetectInput(void) {
	/* Detects keyboard input and mouse movement */
	devInputKB ->Acquire();
	devInputKB ->GetDeviceState(256, (LPVOID)keystate);
}
VOID CleanD3D(void) {
	/* Releases D3D devices and objects */
	for(int i = 0; i < BrickCount; i++) {
		bricks[i] ->Cleanup();
	}
	d3dDevice ->Release();
	d3dObject ->Release();
	fntPong ->Release();
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
FLOAT LoadLevel(void) {
	FILE* fp;
	INT i, buf2[3], ii, tmp, tmp2;
	float buf;
	char bufchar[16];
	if(strcmp(__argv[0], "c:\\Users\\Elias\\Documents\\Visual Studio 2008\\Projects\\Pong\\Debug\\Pongout.exe") == 0) {
		fp = fopen("c:\\Users\\Elias\\Documents\\Visual Studio 2008\\Projects\\Pong\\Release\\data.djm", "r");
	}
	else {
		fp = fopen("data.djm", "r");
	}
		for(i = 0; i < 16; i++) {
			bufchar[i] = (char)fgetc(fp);
		}
		if(strcmp(bufchar, "PG-DJM;28/11/09;") != 1) {
			MessageBox(0, "You have a corrupted version. Please re-download this game from the site you found it at.",
						"Information", MB_ICONINFORMATION | MB_OK);
		}
		buf = (float)fgetc(fp);
		ENEMY_DIFFICULTY = (float)fgetc(fp) + ((float)fgetc(fp) / 100);
		
		tmp = fgetc(fp);
		if(tmp != 0) {
			for(i = 0; i < tmp; i++) {
				buf2[0] = fgetc(fp);
				tmp2 = fgetc(fp);
				buf2[1] = fgetc(fp);
				for(ii = 0; ii < tmp2 - 1; ii++) {
					buf2[1] += fgetc(fp);
				}
				buf2[2] = fgetc(fp);
				for(ii = 0; ii < tmp2 - 1; ii++) {
					buf2[2] += fgetc(fp);
				}
				bricks[i + BrickCount] = new BRICK((float)buf2[1], (float)buf2[2], BRICKSPRS[buf2[0]]);
			}
			BrickCount += tmp;
		}
	fclose(fp);
	return buf;
}
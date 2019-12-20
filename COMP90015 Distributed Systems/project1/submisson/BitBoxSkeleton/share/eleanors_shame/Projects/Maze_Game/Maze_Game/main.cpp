#include <dxhandler.h>
#include <windows.h>
#include <vector>
#include <fstream>
#include <stdx.h>
#include <ctime>
#include <mmsystem.h>
#pragma comment(lib, "Winmm.lib")

const int player = 0;
const int wall = 1;
const int ladder = 2;
class Player: public dxh::OBJECT {
	public:
		__forceinline Player(): wait(0) { no_arg_ctor(); }
		Player(D3DXVECTOR3 pos): wait(20) { ctor("\\sprites\\hat.png", 1, pos); type = 0; }
		void step_ev() {
			if(dxh::key_check(VK_LEFT) && alarm[0] <= 0 && !place_meeting(x - 32, y, wall)) {
				xyz.x -= 32;
				alarm[0] = wait;
			}
			if(dxh::key_check(VK_RIGHT) && alarm[1] <= 0 && !place_meeting(x + 32, y, wall)) {
				xyz.x += 32;
				alarm[1] = wait;
			}
			if(dxh::key_check(VK_UP) && alarm[2] <= 0 && !place_meeting(x, y - 32, wall)) {
				xyz.y -= 32;
				alarm[2] = wait;
			}
			if(dxh::key_check(VK_DOWN) && alarm[3] <= 0 && !place_meeting(x, y + 32, wall)) {
				xyz.y += 32;
				alarm[3] = wait;
			}
		}
	private:
		const short wait;
};
class Wall: public dxh::OBJECT {
	public:
		Wall() { no_arg_ctor(); }
		Wall(D3DXVECTOR3 pos) { ctor("\\sprites\\wall.png", 1, pos); type = 1; }
};
class Ladder: public dxh::OBJECT {
	public:
		Ladder() { no_arg_ctor(); }
		Ladder(D3DXVECTOR3 pos) { ctor("\\sprites\\ladder.png", 1, pos); type = 2; }
};


std::vector<Player*> PlayerObjs;
std::vector<Wall*> WallObjs;
std::vector<Ladder*> LadderObjs;

void load_level(std::string filename);
void render(dxh::DXHANDLER& handler, unsigned fps = 60);
INT APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd) {
	unused(hInstance);
	unused(hPrevInstance);
	unused(lpCmdLine);
	unused(nShowCmd);
	
	HWND hWnd;
	dxh::DXHANDLER handler(hWnd, "Maze Game");

	load_level("\\level1.djm");

	dxh::set_clear_col(0);
	handler.render();

	dxh::show_window(hWnd);
	MSG msg;
	ZeroMemory(&msg, sizeof(msg));
	while(msg.message != WM_QUIT) {
		if(PeekMessage(&msg, 0, 0, 0, PM_REMOVE)) {
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		if(dxh::key_check(VK_ESCAPE)) PostQuitMessage(0);
		render(handler);
	}
	return msg.wParam;
}
void load_level(std::string filename) {
	filename = dxh::wdir + filename;
	if(!stdx::fexists(filename.c_str())) {
		MessageBoxA(0, ("Level \"" + filename + "\" does not exist.").c_str(), "Error", MB_ICONERROR | MB_OK);
		return;
	}
	std::ifstream fp;
	fp.open(filename.c_str(), std::ios::in | std::ios::binary | std::ios::ate);
	if(!fp.is_open()) {
		MessageBoxA(0, ("An unknown error occured while loading level \"" + filename + "\".").c_str(), "Error", MB_ICONERROR | MB_OK);
		return;
	}
	unsigned sz = fp.tellg();
	char* mbk = new char[sz];
	if(!mbk) {
		MessageBoxA(0, "Memory allocation error; computer may be out of memory.", "Error", MB_ICONERROR | MB_OK);
		return;
	}
	fp.seekg(0, std::ios::beg);
	fp.read(mbk, sz);
	fp.close();
	unsigned i;
	{
		std::string header;
		for(i = 0; i < 16; ++i) {
			header += mbk[i];
		}
		if(header != "PG-DJM;11/12/09;") {
			MessageBoxA(0, ("Level \"" + filename + "\" is invalid.").c_str(), "Error", MB_ICONERROR | MB_OK);
			return;
		}
	}
	for(; i < sz; ++i) {
		int type = mbk[i];
		float x = (float)mbk[++i] * (float)16;
		float y = (float)mbk[++i] * (float)16;
		D3DXVECTOR3 pos(x, y, 0.0f);
		switch(type) {
			case 0:
				PlayerObjs.push_back(new Player(pos));
				break;
			case 1:
				WallObjs.push_back(new Wall(pos));
				break;
			case 2:
				LadderObjs.push_back(new Ladder(pos));
		}
	}
	delete[] mbk;
	mbk = 0;
}
void render(dxh::DXHANDLER& handler, unsigned fps) {
	static DWORD last_frame = timeGetTime();
	DWORD c_time = timeGetTime();
	if(c_time - last_frame < 1000u / fps) {
		return;
	}
	last_frame += 1000 / fps;
	handler.render();
}
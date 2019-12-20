#define _CRTDBG_MAP_ALLOC
#include <iostream>
#include <process.h>
#include <glutwrap.h>
#include <pngload.h>
#include "player.h"

void _t_loadimg (void* v_glut_ptr);

int main (int argc, char **argv)
{
	std::cout << "Please press enter to start.";
	std::cin.get();
	
	CGlutWrapper2D glut;

	//HANDLE thread = (HANDLE)_beginthread(_t_loadimg, 0, (void*)&glut);

	glut.begin(argc, argv);
	CSprite::default_func = CPNGLoader::load_png;
	CSprite::set_texture("C:/C/sprites/mario/sprite.png");

	//WaitForSingleObject(thread, 100);

	glut.room_load_bmp("C:/C/sprites/mario/level1.bmp", 16);
	glut.tile_load_png("C:/C/sprites/mario/tiles.png", 16);
	
	glut.main_loop();
	//while (glut.step()) { }
	return 0;
}

void _t_loadimg (void* v_glut_ptr)
{
	CGlutWrapper2D* glut_ptr = (CGlutWrapper2D*)v_glut_ptr;
	CGlutWrapper2D& glut = *glut_ptr;
	glut.room_load_bmp("C:/C/sprites/mario/level1.bmp", 16);
	glut.tile_load_png("C:/C/sprites/mario/tiles.png", 16);
}
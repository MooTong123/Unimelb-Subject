#define _CRTDBG_MAP_ALLOC
#pragma once
#include "player.h"

class CPlatform: public CObject
{
};
class CPlank: public CPlatform
{
	public:
		CPlank () { add(); collision_sprite.assign(96, 32, 16, 16); }
		int get_type () { return obj_plank; }
		void step () {}
};
class CMetal: public CPlatform
{
	public:
		CMetal () {
			add();
			has_default = false;
			sprite.assign(32, 32, 64, 16, 4, 1);
		}
		virtual int get_type () { return obj_metal; }
		virtual void step () { 
			/*if (sprite.get_image() == 1) {
				sprite.set_speed(4);
			}
			else if (sprite.get_image() == 0) {
				sprite.set_speed(0.1);
			}*/
		}
		void draw () { sprite.draw((float)x, (float)y); }

		float get_width () const { return 16; }
		float get_height () const { return 16; }

	protected:
		CAnimation sprite;
};
class CItemBlock: public CMetal
{
	public:
		CItemBlock (): used(false) {
			add();
			has_default = false;
			sprite.assign(0, 48, 64, 16, 4, 1);
		}
		void step () { 
			if (!used) {
				sprite.set_speed(1);
			}
			sprite.step();
		}
		int get_type () {
			return obj_itemblock;
		}

		void create_item () {
			if (!used) {
				glut.instance_create(obj_mushroom, x, y - 16);
				used = true;
				sprite.assign(64, 48, 16, 16, 1, 1);
				sprite.set_speed(0);
				sprite.set_image(0);
			}
		}

		void draw () { sprite.draw((float)x, (float)y); }

	private:
		bool used;
};
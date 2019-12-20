#pragma once
#define _CRTDBG_MAP_ALLOC
#include <glutwrap.h>
#include "player.h"

class CEnemy: public CObject {
};
class CSquareThing: public CEnemy
{
	public:
		CSquareThing ();
		void step ();
		void draw () { sprite.draw((float)x, (float)y, velocity.x, vspeed); }
		int get_type () { return obj_squarething; }
		void reverse () { velocity.x = -velocity.x; }

		float get_width () const { return 16; }
		float get_height () const { return 16; }

	private:
		bool falling;
		double gravity_max;
		double vspeed;
		double gravity;
		CAnimation sprite;
};
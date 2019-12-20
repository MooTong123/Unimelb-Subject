#include <glutwrap.h>
#include "player.h"

class CMushroom: public CObject
{
	public:
		CMushroom ();
		void step ();
		void draw () { sprite.draw((float)x, (float)y); }
		int get_type () { return obj_mushroom; }

		float get_width () const { return 20; }
		float get_height () const { return 16; }
			
	private:
		bool falling;
		double gravity_max;
		double vspeed;
		double gravity;
		CAnimation sprite;
};
#define _CRTDBG_MAP_ALLOC
#include <glutwrap.h>
#include "enemy.h"

CSquareThing::CSquareThing (): falling(false), gravity_max(10), vspeed(0), gravity(0)
{
	add();
	has_default = false;
	depth = 1;
	sprite.assign(0, 32, 32, 16, 2, 1);
	sprite.set_speed(2);

	velocity.x = choose<double>(-4, 4);
}
void CSquareThing::step ()
{
	for (int i = 0; !place_free(x, y) && i < 16; ++i) {
		y--;
		vspeed = 0;
		gravity = 0;
		falling = false;
	}
	if (place_meeting(x + velocity().x, y, gw::CRange(obj_plank, obj_metal))) {
		velocity.x = -velocity.x;
	}

	if (place_free(x, y + 16) && falling == false) {
		falling = true;
		gravity = gravity_max;
	}

	y -= vspeed * gw::dt();
	vspeed -= gravity * gw::dt();

	if (falling == true && sign<double>(vspeed) == -1 && collides_with(gw::CRange(obj_plank, obj_metal))) {
		vspeed = 0;
		gravity = 0;
		falling = false;
	}

	if (sprite.get_speed() > 5 || sprite.get_speed() == 0) {
		sprite.set_speed(5);
	}

	x += velocity().x;

	sprite.step();
}

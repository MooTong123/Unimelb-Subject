#include <glutwrap.h>
#include "pngload.h"

enum objs {
	obj_player,
	obj_block
};

class Player: public CObject
{
	public:
		Player ();
		void step ();
		void draw () { glut.draw_rectangle(x, y, 16, 16, COLOUR(1, 0, 0), glut.tv_empty, 0, 0); }
		
		int get_type () { return obj_player; }
		float get_width () const { return 16; }
		float get_height () const { return 16; }

	private:
		gw::CVector velocity;
		gw::CVector acceleration;
		gw::CVector dir;
		const gw::CVector max_velocity;
		const double jump_speed;
		const double gravity_max;
		double gravity;

		bool falling;
};
class Block: public CObject
{
	public:
		Block () { add(); }
		void step () {}
		void draw () { glut.draw_rectangle(x, y, 16, 16, BLACK, glut.tv_empty, 0, 0); }
};
CObject* allocator(t_class type)
{
	switch (type) {
		case obj_player:
			return new Player;
			break;
		default:
			return new Block;
			break;
	}
}

int main (int argc, char** argv)
{
	CGlutWrapper2D glut;
	CSprite::default_func = CPNGLoader::load_png;
	CSprite::set_texture("C:/C/sprites/mario/sprite.png");
	glut.begin(argc, argv);

	glut.room_load_bmp("C:/C/sprites/mario/level1.bmp", 16);

	glut.main_loop();

	return 0;
}

Player::Player (): jump_speed(25), gravity_max(2), gravity(0), falling(false),
	max_velocity(15, 25), dir(1, 1)
{
	add();
	glut.clear_colour(108, 137, 254);
	for (int i = 0; !place_free(x, y) && i < 16; ++i) {
		y--;
	}
}
void Player::step ()
{
	for (int i = 0; !place_free(x, y) && i < 16; ++i) {
		y--;
		velocity.y = 0;
		acceleration.y = 0;
		gravity = 0;
		falling = false;
	}
	if (glut.get_key(GLUT_KEY_ESC)) {
		glut.end();
	}

	if (glut.get_key_ex(GLUT_KEY_RIGHT)) {
		acceleration.x += 1 * gw::dt();
		dir.x = 1;
	}
	if (glut.get_key_ex(GLUT_KEY_LEFT)) {
		acceleration.x += 1 * gw::dt();
		dir.x = -1;
	}

	CViewInfo view;
	view.x = (float)x - 720;
	view.y = (float)y - 450;
	glut.modify_view(view);

	if (glut.get_key_pressed(' ') && falling == false) {
		dir.y = -1;
		falling = true;
		velocity.y = jump_speed;
		gravity = gravity_max;
	}
	if (glut.get_key_released(' ')) {
		velocity.y *= 0.75;
	}

	if (place_free(x, y + 16) && falling == false) { // no floating in air
		falling = true;
		gravity = gravity_max;
	}
	if (sign(velocity.y) == 1 && place_meeting(x, y - abs(velocity.y * gw::dt()), obj_block)) { // collide with roof
		velocity.y = 0;
		acceleration.y = 0;
	}

	if (velocity.x != 0) {
		acceleration.x -= 0.5;
	}
	if (falling) {
		velocity.y -= gravity;
	}

	if (velocity.x > max_velocity.x) {
		velocity.x = max_velocity.x;
	}

	velocity += acceleration;
	if (velocity.y != 0) {
		y += velocity.y * dir.y;
	}
	if (!place_meeting(x + velocity.x * dir.x, y, obj_block)) {
		x += velocity.x * dir.x;
	}
	else if (place_free(x, y)) {
		velocity.x = 0;
		acceleration.x = 0;
	}

	if (falling == true && place_meeting(x, y, obj_block)) { // collide with floor
		acceleration.y = 0;
		velocity.y = 0;
		gravity = 0;
		falling = false;
	}
}
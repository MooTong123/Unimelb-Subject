#define _CRTDBG_MAP_ALLOC
#include <glutwrap.h>
#include <sstream>
#include "player.h"
#include "platform.h"
#include "enemy.h"
#include "item.h"

const int block_size = 16;

CObject* allocator (t_class type)
{
	switch (type) {
		case obj_player:
			return new CPlayer;
			break;
		case obj_plank:
			return new CPlank;
			break;
		case obj_metal:
			return new CMetal;
			break;
		case obj_squarething:
			return new CSquareThing;
			break;
		case obj_itemblock:
			return new CItemBlock;
			break;
		case obj_mushroom:
			return new CMushroom;
			break;
	}
	return nullptr;
}
float CPlayer::get_width () const {
	return current_sprite->get_width();
}
float CPlayer::get_height () const {
	return current_sprite->get_height();
}
CPlayer::CPlayer (): jump_speed(25), gravity_max(2), gravity(0), falling(false),
	current_sprite(&sprite_right), max_velocity(15, 25), dir(1, 1), running(false)
{
	add();
	has_default = false;
	sprite_right.assign(0, 0, 120, 16, 10, 1);
	sprite_left.assign(0, 16, 120, 16, 10, 1);
	sprite_big_right.assign(82, 64, 70, 64, 5, 2);
	sprite_big_left.assign(120, 0, 70, 64, 5, 2);
	sprite_jump_left.assign(13, 96, 14, 16, 1, 1);
	sprite_jump_right.assign(0, 96, 14, 16, 1, 1);
	sprite_jump_big_left.assign(27, 96, 17, 30, 1, 1);
	sprite_jump_big_right.assign(44, 96, 17, 30, 1, 1);

	glut.clear_colour(108, 137, 254);
	for (int i = 0; !place_free(x, y) && i < block_size; ++i) {
		y--;
	}
}
void CPlayer::step ()
{
	for (int i = 0; !place_free(x, y) && i < block_size; ++i) {
		y--;
		velocity.y = 0;
		acceleration.y = 0;
		gravity = 0;
		falling = false;
	}

	if (glut.get_key(GLUT_KEY_ESC)) {
		glut.end();
	}

	if (glut.get_key_ex(GLUT_KEY_RIGHT)) { // movement handling
		if (falling) {
			if (state == st_big) {
				current_sprite = &sprite_jump_big_right;
			}
			else {
				current_sprite = &sprite_jump_right;
			}
			current_sprite->set_speed(0);
		}
		else {
			if (state == st_big) {
				current_sprite = &sprite_big_right;
			}
			else {
				current_sprite = &sprite_right;
			}
			current_sprite->set_speed(60);
		}
		acceleration.x += 1 * gw::dt();
 		dir.x = 1;
	}
	else if (glut.get_key_ex(GLUT_KEY_LEFT)) {
		if (falling) {
			if (state == st_big) {
				current_sprite = &sprite_jump_big_left;
			}
			else {
				current_sprite = &sprite_jump_left;
			}
			current_sprite->set_speed(0);
		}
		else {
			if (state == st_big) {
				current_sprite = &sprite_big_left;
			}
			else {
				current_sprite = &sprite_left;
			}
			current_sprite->set_speed(-60);
		}
		acceleration.x += 1 * gw::dt();
		dir.x = -1;
	}
	else {
		current_sprite->set_speed(0);
	}

	CViewInfo view;
	view.x = (float)x - 720;
	view.y = (float)y - 450;
	glut.modify_view(view);

	if (glut.get_key_pressed(' ') && falling == false) { // jumping
		dir.y = -1;
		if (current_sprite == &sprite_big_left) {
			current_sprite = &sprite_jump_big_left;
		}
		else if (current_sprite == &sprite_left) {
			current_sprite = &sprite_jump_left;
		}
		else if (current_sprite == &sprite_big_right) {
			current_sprite = &sprite_jump_big_right;
		}
		else if (current_sprite == &sprite_right) {
			current_sprite = &sprite_jump_right;
		}
		falling = true;
		velocity.y = jump_speed;
		gravity = gravity_max;
	}
	if (glut.get_key_released(' ')) {
		velocity.y *= 0.75;
	}
	/*if (glut.get_key(' ')// spacebar down
		&& falling == true // in air
		&& vspeed < jump_max // not too fast
		&& place_free(x, y)) { // haven't hit floor
		vspeed += jump_increase * gw::dt();
	}*/

	if (glut.get_key_pressed_ex(GLUT_KEY_SHIFT)) {
		running = true;
	}
	if (glut.get_key_released_ex(GLUT_KEY_SHIFT)) {
		running = false;
	}


	if (place_free(x, y + 1) && falling == false) { // no floating in air
		falling = true;
		gravity = gravity_max;
	}

	if (sign<double>(acceleration.y) == 1 && place_meeting(x, y - abs(velocity.y * gw::dt()), obj_itemblock)) { // collide with roof
		velocity.y = 0;
		instance_nearest<CItemBlock>(obj_itemblock)->create_item();
	}
	if (sign<double>(velocity.y) == 1 && place_meeting(x, y - abs(velocity.y * gw::dt()), gw::CRange(obj_plank, obj_metal))) { // collide with roof
		velocity.y = 0;
	}

	acceleration.x -= 0.5 * gw::dt();
	acceleration.y -= gravity * gw::dt();

	velocity += acceleration * gw::dt();

	if (velocity.x > max_velocity.x) {
		velocity.x = max_velocity.x;
	}
	if (velocity.x < 0) {
		velocity.x = 0;
	}
	if (velocity.y < 0) {
		velocity.y = 0;
	}

	if (sign(velocity.x) != sign(dir.x)) {
		velocity.x *= dir.x;
	}
	if (sign(velocity.y) != sign(dir.y)) {
		velocity.y *= dir.y;
	}

	y += velocity.y;

	if (!place_meeting(x + velocity.x * dir.x, y, gw::CRange(obj_plank, obj_metal))) {
		x += velocity.x;
	}
	else if (place_free(x, y)) {
		velocity.x = 0;
		acceleration.x = 0;
	}

	if (falling == true && sign<double>(velocity.y) == -1 && collides_with(gw::CRange(obj_plank, obj_metal))) { // collide with floor
		acceleration.y = 0;
		velocity.y = 0;
		gravity = 0;
		falling = false;
		if (current_sprite == &sprite_jump_big_left) {
			current_sprite = &sprite_big_left;
		}
		else if (current_sprite == &sprite_jump_left) {
			current_sprite = &sprite_left;
		}
		else if (current_sprite == &sprite_jump_big_right) {
			current_sprite = &sprite_big_right;
		}
		else if (current_sprite == &sprite_jump_right) {
			current_sprite = &sprite_right;
		}
	}
	
	CSquareThing* enemy;
	enemy = (CSquareThing*)get_collision(obj_squarething);
	if (enemy != noone) {
		if (y <= enemy->get_y()) {
			enemy->die();
			if (glut.get_key(' ')) {
				acceleration.y = jump_speed;
			}
			else {
				acceleration.y = jump_speed / 2;
			}
		}
		else {
			std::cout << "you should be dead...\n";
			enemy->reverse();
		}
	}
	CMushroom* mush;
	mush = (CMushroom*)get_collision(obj_mushroom);
	if (mush != noone) {
		mush->die();
		state = st_big;
		y -= 16;
		if (current_sprite == &sprite_left) {
			current_sprite = &sprite_big_left;
		}
		else if (current_sprite == &sprite_right) {
			current_sprite = &sprite_big_right;
		}
	}
	current_sprite->step();
}

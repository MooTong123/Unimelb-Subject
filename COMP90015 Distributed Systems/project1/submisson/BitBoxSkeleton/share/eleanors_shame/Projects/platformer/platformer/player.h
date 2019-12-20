#pragma once
#include <glutwrap.h>

enum objects {
	obj_player,
	obj_plank,
	obj_metal,
	obj_squarething,
	obj_itemblock,
	obj_mushroom
};

class CPlayer: public CObject
{
	public:
		CPlayer ();
		void step ();
		int get_type () { return obj_player; }

		NO_ASSIGN(CPlayer);

		float get_width () const;
		float get_height () const;

		void draw () { current_sprite->draw((float)x, (float)y); }

	private:
		const double jump_speed;
		const double gravity_max;
		double gravity;

		gw::CVector velocity;
		gw::CVector acceleration;
		gw::CVector dir;
		const gw::CVector max_velocity;

		bool falling;
		bool running;

		enum p_states {
			st_normal,
			st_big,
			st_fire
		} state;

		CAnimation sprite_left;
		CAnimation sprite_right;
		CAnimation sprite_big_left;
		CAnimation sprite_big_right;
		CAnimation sprite_jump_left;
		CAnimation sprite_jump_right;
		CAnimation sprite_jump_big_left;
		CAnimation sprite_jump_big_right;
		CAnimation *current_sprite;
};
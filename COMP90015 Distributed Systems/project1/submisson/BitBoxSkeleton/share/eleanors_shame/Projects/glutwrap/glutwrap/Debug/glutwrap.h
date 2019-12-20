////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//																																													  //
//		Glut Wrapper 1.0.0 - Jack McMurtry 15/06/2011																																  //
//		This library contains several classes designed to aid the use of OpenGL and GLUT in a 2D environment. Applications can														  //
//		be quickly and easily developed in less than 100 lines.																														  //
//																																													  //
//		Typedefs:																																									  //
//		byte																																										  //
//			An unsigned char redefined to byte for clarity.																															  //
//		t_load (std::vector <byte> &, unsigned long &, unsigned long &, const string &)																								  //
//			A function pointer used for loading sprites.																															  //
//																																													  //
//		Structs:																																									  //
//			COLOUR																																									  //
//				Variables:																																							  //
//					float red																																						  //
//					float green 																																					  //
//					float blue   																																					  //
//				Stores the colour information. The struct contains constructors for aid.																							  //
//																																													  //
//		Classes:																																									  //
//			CGlutWrapper2D																																							  //
//				Purpose:																																							  //
//					This class is the bulk of the code. It wraps the initialisation and rendering. One object of this class															  //
//					should always be present.																																		  //
//				Functions:																																							  //
//					CGlutWrapper2D ()																																				  //
//						A constructor that initialises the necessary variables in the various classes with default values. It takes no arguments.									  //
//					CGlutWrapper2D (int w, int h, int x, int y, string name, COLOUR c)																								  //
//						A constructor that initialises variables with chosen values.																								  //
//					COLOUR colour ()																																				  //
//						Returns the current clear colour.																															  //
//					void colour (COLOUR col)																																		  //
//					void colour (float red, float green, float blue)																												  //
//						Functionally equivalent; sets the colour the screen is cleared to either using a COLOUR struct or floats.													  //
//					

#pragma once
#ifndef _GLUTWRAP_H
#define _GLUTWRAP_H

#define GLUT_NO_WARNING_DISABLE

#include <cstdlib>
#include <windows.h>
#include <glut.h>
#include <gl/gl.h>
#include <gl/glu.h>
#include <stack>
#include <vector>
#include <string>
#include <exception>
#include <iostream>

#define register _register
#define UNUSED
#define _Inline

typedef void(*t_load) (std::vector<byte>&, unsigned long&, unsigned long&, const std::string&);
typedef unsigned char byte;

const byte GLUT_KEY_ESC = 27;

class CGlutWrapper2D;
class CObject;
class CSpriteMultiple;
class CSpriteSingle;

class CTextureVector
{
	public:
		CTextureVector (): _empty(true) {}
		void push_back (float a, float b) { _empty = false; v.push_back(std::pair<float, float>(a, b)); }
		float at_a (unsigned i) const { return (v[i]).first; }
		float at_b (unsigned i) const { return (v[i]).second; }
		bool empty () const { return _empty; }
		void clear () { v.clear(); }

		void operator=(int x) { UNUSED(x); }
	private:
		std::vector <std::pair<float, float>> v;
		bool _empty;
};
struct COLOUR
{
	float red;
	float green;
	float blue;
	COLOUR(float r, float g, float b) { red = r; green = g; blue = b; }
	COLOUR() { red = 0.0f; green = 0.0f; blue = 0.0f; }
};

struct CSprite
{
	static t_load default_func;
};

class CSpriteMultiple
{
	public:
		CSpriteMultiple (): width(0), height(0), texture(0) {}
		CSpriteMultiple (std::string filename, t_load ld = CSprite::default_func);
		~CSpriteMultiple () { glDeleteTextures(1, &texture); }

		void assign (std::string filename, t_load load_func = CSprite::default_func);
		void draw (CGlutWrapper2D& gwrap, float x, float y);

		friend class CGlutWrapper2D;
		friend class CSpriteSingle;

	private:
		std::vector<byte>			image;
		unsigned long				width;
		unsigned long				height;
		unsigned int				texture;

		static int					tex_n;

		void load (std::string filename, t_load load_func = CSprite::default_func);
};

class CGlutWrapper2D
{
	public:
		static const CTextureVector tv_empty;

		CGlutWrapper2D (const int w, const int h, const int x, const int y, std::string name, COLOUR c);
		CGlutWrapper2D ();

		bool get_key (byte key) const;
		bool get_key_ex (int key) const;
		COLOUR clear_colour () const;
		void clear_colour (COLOUR c);
		void clear_colour (float r, float g, float b);
		void draw_triangle (float x, float y, float width, float height, COLOUR col, const CTextureVector& t_vector, unsigned int texture);
		void draw_rectangle (float x, float y, float width, float height, COLOUR col, const CTextureVector& t_vector, unsigned int texture);
		void begin (int& argc, char**& argv) const;
		void end (int return_code = 0) const { exit(return_code); }

		static CGlutWrapper2D& get_ptr () { return *_static_ptr; }

	private:
		friend class CObject;

		static CGlutWrapper2D *_static_ptr;
		void add_obj (CObject& obj) { obj_array.push_back(&obj); ++objs; }
		void key_down (byte key, int x, int y);
		void key_up (byte key, int x, int y);
		void key_down_ex (int key, int x, int y);
		void key_up_ex (int key, int x, int y);
		void reshape (const int w, const int h);
		void render ();

		static void handle_errors (bool shownoerrors = false);

		int window_width;
		int window_height;
		int window_x;
		int window_y;
		std::string window_name;
		std::stack <float> render_stack;
		std::stack <CTextureVector> tex_coord_stack;
		unsigned int objs;
		std::vector <CObject*> obj_array;

		COLOUR clear_col;

		void construct ()
		{
			CSpriteMultiple::tex_n = 1;
			if (CGlutWrapper2D::_static_ptr != NULL)
				throw std::runtime_error("Only one instance of CGlutWrapper2D");
			CGlutWrapper2D::_static_ptr = this;
		}

		bool keyboard[256];
		bool keyboard_ex[1000];

		void render_push (COLOUR col, unsigned int polygon, const std::vector<float>& vertices, const CTextureVector& t_vector, unsigned int texture);
		void render_pop ();
		void orth_start () const;
		void orth_end () const;

		class callback;
};
class CGlutWrapper2D::callback
{
	public:
		static inline void display () { _static_ptr->render(); }
		static inline void reshape (int w, int h) { _static_ptr->reshape(w, h); }
		static inline void dkey (byte key, int x, int y) { _static_ptr->key_down(key, x, y); }
		static inline void ukey (byte key, int x, int y) { _static_ptr->key_up(key, x, y); }
		static inline void dkeyx (int key, int x, int y) { _static_ptr->key_down_ex(key, x, y); }
		static inline void ukeyx (int key, int x, int y) { _static_ptr->key_up_ex(key, x, y); }
};

class CSpriteSingle
{
	public:
		CSpriteSingle (): x(-1), y(-1), w(-1), h(-1) { tv = CGlutWrapper2D::tv_empty; }
		CSpriteSingle (float xpos, float ypos, float width, float height): x(xpos), y(ypos), w(width), h(height) { load(x, y, w, h); tv = CGlutWrapper2D::tv_empty; }
		~CSpriteSingle () { glDeleteTextures(1, &single_texture); }

		void assign (float xpos, float ypos, float width, float height);
		void draw (CGlutWrapper2D& gwrap, float xpos, float ypos);
		void set_tex_coords (const CTextureVector& v) { tv = v; }

		static void set_texture (const std::string& filename, t_load load_func = CSprite::default_func);

		friend class CGlutWrapper2D;

	private:
		static unsigned int single_texture;
		static unsigned long width;
		static unsigned long height;
		float x;
		float y;
		float w;
		float h;
		CTextureVector tv;

		void load (float xpos, float ypos, float ww, float hh);
};

class CObject
{
	protected:
		friend class CGlutWrapper2D;

		virtual void step () = 0;
		void register (CGlutWrapper2D &gwrap) {
			gwrap.add_obj(*this);
		}
};

const COLOUR WHITE(1.0f, 1.0f, 1.0f);
const COLOUR BLACK(0.0f, 0.0f, 0.0f);

#endif

#pragma once

#include "glutwrap.h"
#include <cstdio>
#include <windows.h>
#include "glut.h"
#include <gl/gl.h>
#include <gl/glu.h>
#include <stack>
#include <vector>
#include <utility>
#include <string>
#include <iostream>
#include <fstream>

int CSpriteMultiple::tex_n =							1;
unsigned int CSpriteSingle::single_texture =			0;
unsigned long	CSpriteSingle::width =					0;
unsigned long CSpriteSingle::height =					0;
t_load CSprite::default_func =							0;
CGlutWrapper2D *CGlutWrapper2D::_static_ptr =			NULL;
const CTextureVector CGlutWrapper2D::tv_empty =			CTextureVector();

inline
void glutClearColour(COLOUR c, float alpha) { glClearColor(c.red, c.green, c.blue, alpha); }
inline
void glutClearColour(COLOUR c) { glClearColor(c.red, c.green, c.blue, 1.0f); }

void CGlutWrapper2D::orth_start () const
{
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();

	gluOrtho2D(0, window_width, 0, window_height);
	glScalef(1, -1, 1);
	glTranslatef(0, -(float)window_height, 0);

	glMatrixMode(GL_MODELVIEW);
}
void CGlutWrapper2D::orth_end () const
{
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);
}
void CGlutWrapper2D::render ()
{
	float red, green, blue, x, y, type;
	unsigned int i, j, tex;
	CTextureVector t_coord;
	t_coord.push_back(0.0f, 1.0f);
	t_coord.push_back(1.0f, 1.0f);
	t_coord.push_back(1.0f, 0.0f);
	t_coord.push_back(0.0f, 0.0f);
	static const CTextureVector _copy = t_coord;

	for (i = 0u; i < objs; ++i)
		obj_array.at(i)->step();
	
	glutClearColour(clear_col);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();

	orth_start();

		while(!render_stack.empty()) {
			red = render_stack.top();
			render_pop();
			green = render_stack.top();
			render_pop();
			blue = render_stack.top();
			render_pop();

			glColor3f(red, green, blue);

			type = render_stack.top();
			if (type == GL_TRIANGLES) {
				i = 3;
			}
			else {
				i = 4;
			}
			j = i - 1;
			render_pop();

			tex = (int)render_stack.top();
			render_pop();

			glBindTexture(GL_TEXTURE_2D, tex);
			glBegin((GLenum)type);
				if(!tex_coord_stack.empty()) {
					t_coord = tex_coord_stack.top();
					tex_coord_stack.pop();
				}
				else {
					t_coord = _copy;
				}
				while (i > 0) {
					--i;

					x = render_stack.top();
					render_pop();
					y = render_stack.top();
					render_pop();
					if (type == GL_QUADS)
						glTexCoord2f(t_coord.at_a(i), t_coord.at_b(i));
					glVertex2f(x, y);
				}
			glEnd();
			CGlutWrapper2D::handle_errors();
		}

	orth_end();

	glutSwapBuffers();
}
void CGlutWrapper2D::render_push (COLOUR col, unsigned int polygon, const std::vector<float>& vertices, const CTextureVector& t_vector, unsigned int texture)
{
	int i;
	for(i = vertices.size() - 1; i >= 0; --i) {
		render_stack.push(vertices[i]);
	}
	render_stack.push((float)texture);

	render_stack.push((float)polygon);

	render_stack.push(col.red);
	render_stack.push(col.green);
	render_stack.push(col.blue);

	if (!t_vector.empty())
		tex_coord_stack.push(t_vector);
}
void CGlutWrapper2D::draw_triangle (float x, float y, float width, float height, COLOUR col, const CTextureVector& t_vector, unsigned int texture) {
	std::vector <float> vertices;
	vertices.push_back(x);
	vertices.push_back(y);
	vertices.push_back(x + width);
	vertices.push_back(y);
	vertices.push_back(x + width);
	vertices.push_back(y + height);
	render_push(col, GL_TRIANGLES, vertices, t_vector, texture);
}
void CGlutWrapper2D::draw_rectangle (float x, float y, float width, float height, COLOUR col, const CTextureVector& t_vector, unsigned int texture) {
	std::vector <float> vertices;
	vertices.push_back(x);
	vertices.push_back(y);
	vertices.push_back(x + width);
	vertices.push_back(y);
	vertices.push_back(x + width);
	vertices.push_back(y + height);
	vertices.push_back(x);
	vertices.push_back(y + height);
	render_push(col, GL_QUADS, vertices, t_vector, texture);
}

void CGlutWrapper2D::key_down (byte key, int x, int y)
{
	UNUSED(x);
	UNUSED(y);
	keyboard[key] = 1;
}
void CGlutWrapper2D::key_up (byte key, int x, int y)
{
	UNUSED(x);
	UNUSED(y);
	keyboard[key] = 0;
}
bool CGlutWrapper2D::get_key (byte key) const
{
	return (keyboard[key] == 1);
}
void CGlutWrapper2D::key_down_ex (int key, int x, int y)
{
	UNUSED(x);
	UNUSED(y);
	keyboard_ex[key] = 1;
}
void CGlutWrapper2D::key_up_ex (int key, int x, int y)
{
	UNUSED(x);
	UNUSED(y);
	keyboard_ex[key] = 0;
}
bool CGlutWrapper2D::get_key_ex (int key) const
{
	return (keyboard_ex[key] == 1);
}

void CGlutWrapper2D::reshape (const int w, const int h)
{
	glViewport(0, 0, (GLsizei)w, (GLsizei)h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(60, (GLfloat)w / (GLfloat)h, 0.1, 1000.0);
	window_width = w;
	window_height = h;
	glMatrixMode(GL_MODELVIEW);
}

void CGlutWrapper2D::handle_errors (bool shownoerrors)
{
	switch (glGetError())
	{
		case GL_NO_ERROR:
			if (shownoerrors) std::cout << "No errors recorded.\n";
			break;
		case GL_INVALID_ENUM:
			std::cout << "Invalid enumeration passed; function ignored\n";
			break;
		case GL_INVALID_VALUE:
			std::cout << "Argument out of range; function ignored\n";
			break;
		case GL_STACK_OVERFLOW:
			std::cout << "Potential stack overflow found; function ignored\n";
			break;
		case GL_STACK_UNDERFLOW:
			std::cout << "Potential stack underflow found; function ignored\n";
			break;
		case GL_OUT_OF_MEMORY:
			std::cout << "Not enough memory available; undefined behaviour\n";
			break;
		default:
			std::cout << "Unknown error\n";
			break;
	}
}

inline
COLOUR CGlutWrapper2D::clear_colour () const { return clear_col; }
inline
void CGlutWrapper2D::clear_colour (COLOUR c) { clear_col = c; }
inline
void CGlutWrapper2D::clear_colour (float r, float g, float b) { clear_colour(COLOUR(r, g, b)); }
inline
void CGlutWrapper2D::render_pop () { render_stack.pop(); }

_Inline
CGlutWrapper2D::CGlutWrapper2D ():
objs(0), window_width(1440), window_height(900), window_x(-1), window_y(-1), window_name("2D Rendering"), clear_col(COLOUR(0, 0, 0))
{ construct(); }
_Inline
CGlutWrapper2D::CGlutWrapper2D (const int w, const int h, const int x, const int y, std::string name, COLOUR c):
objs(0), window_width(w), window_height(h), window_x(x), window_y(y), window_name(name), clear_col(c)
{ construct(); }

void CGlutWrapper2D::begin (int& argc, char**& argv) const
{
	glutInit(&argc, argv);

	glutInitDisplayMode(GLUT_DOUBLE | GLUT_DEPTH | GLUT_RGBA);
	glutInitWindowPosition(window_x, window_y);
	glutInitWindowSize(window_width, window_height);
	glutCreateWindow(window_name.c_str());
	glutIgnoreKeyRepeat(1);

	glutFullScreen();

	glutDisplayFunc(CGlutWrapper2D::callback::display);
	glutIdleFunc(CGlutWrapper2D::callback::display);
	glutReshapeFunc(CGlutWrapper2D::callback::reshape);
	glutKeyboardFunc(CGlutWrapper2D::callback::dkey);
	glutKeyboardUpFunc(CGlutWrapper2D::callback::ukey);
	glutSpecialFunc(CGlutWrapper2D::callback::dkeyx);
	glutSpecialUpFunc(CGlutWrapper2D::callback::ukeyx);

	glEnable(GL_TEXTURE_2D);
	glShadeModel(GL_SMOOTH);
	glClearColor(0.0f, 0.0f, 0.0f, 0.5f);
	glClearDepth(1.0f);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
}

_Inline
CSpriteMultiple::CSpriteMultiple (std::string filename, t_load ld)
{
	load(filename, ld);
}
void CSpriteMultiple::assign (std::string filename, t_load load_func)
{
		load(filename, load_func);
}
void CSpriteMultiple::load (std::string filename, t_load ld)
{
	ld(image, width, height, filename);

	glGenTextures(tex_n, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB8, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, &(image.front()));
	
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	++tex_n;
}
void CSpriteMultiple::draw (CGlutWrapper2D& gwrap, float x, float y)
{
	gwrap.draw_rectangle(x, y, (float)width, (float)height, WHITE, CGlutWrapper2D::tv_empty, texture);
}

void CSpriteSingle::assign (float xpos, float ypos, float width, float height)
{
	load(xpos, ypos, width, height);
}
inline
void CSpriteSingle::load (float xpos, float ypos, float ww, float hh)
{
	if (width == 0)
		throw std::runtime_error("Texture must be assigned before loading sprite");

	x = xpos;
	y = ypos;
	w = ww;
	h = hh;

	tv.push_back(x / width, (y + h) / height);
	tv.push_back((x + w) / width, (y + h) / height);
	tv.push_back((x + w) / width, y / height);
	tv.push_back(x / width, y / height);
}
void CSpriteSingle::set_texture (const std::string& filename, t_load load_func)
{
	std::vector<byte> image;

	unsigned long _width, _height;

	load_func(image, _width, _height, filename);
	glGenTextures(CSpriteMultiple::tex_n, &single_texture);
	glBindTexture(GL_TEXTURE_2D, single_texture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB8, _width, _height, 0, GL_RGBA, GL_UNSIGNED_BYTE, &(image.front()));

	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	CSpriteSingle::width = _width;
	CSpriteSingle::height = _height;

	CSpriteMultiple::tex_n++;
}
void CSpriteSingle::draw (CGlutWrapper2D& gwrap, float xpos, float ypos)
{
	gwrap.draw_rectangle(xpos, ypos, (float)w, (float)h, WHITE, tv, single_texture);
}

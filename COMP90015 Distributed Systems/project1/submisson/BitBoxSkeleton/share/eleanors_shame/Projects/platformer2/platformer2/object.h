#ifndef __OBJECT_H
#define __OBJECT_H
#include <vector>
#include <algorithm>
#include "glwin.h"

typedef unsigned int Id;
class Object
{
	public:
		virtual void update (long double dt) {}
		virtual void render (long double dt) {};
		virtual int getType () const = 0;

		double getx () const { return size.x; }
		double gety () const { return size.y; }
		double getw () const { return size.w; }
		double geth () const { return size.h; }

		void destroy ();

		bool getCollision (int type);
		bool placeFree (double x, double y);
		bool placeMeeting (double x, double y, int type);

		template <class T>
		static T& addObject ();
		template <class T>
		static T& addObject (double x, double y);
		static void updateAll (long double dt);
		static void renderAll (long double dt);
		static void destroyAll ();

	protected:
		static unsigned int objs;

		Object ();

		struct State {
			double x;
			double y;
			double w;
			double h;
			State (): x(0), y(0), w(0), h(0) {}
		} size;

		static std::vector<Object*> objects;

		Id id;

		Object* other;

		GLWindow* win;
};
std::vector<Object*> Object::objects;
unsigned int Object::objs = 0;

Object::Object (): win(GLWindow::getInstance()), other(nullptr)
{
	static bool st = false;
	if (!st) {
		objects.reserve(150);
		st = true;
	}
	unsigned int i;
	// loop i until it finds a null pointer or it's the size of the vector
	for (i = 0; i < objs && objects[i] != nullptr; ++i) ;
	if (i == objs) {
		objs++;
		objects.push_back(this);
		id = objs - 1;
	}
	else {
		objects[i] = this;
		id = i;
	}
}

template <class T>
T& Object::addObject ()
{
	T* obj = new T;
	return *obj;
}
template <class T>
T& Object::addObject (double x, double y)
{
	T* obj = new T;
	obj->size.x = x;
	obj->size.y = y;
	return *obj;
}

template <class T>
T& addObject ()
{
	return Object::addObject<T>();
}
template <class T>
T& addObject (double x, double y)
{
	return Object::addObject<T>(x, y);
}

void Object::updateAll (long double dt)
{
	for (unsigned int i = 0; i < objs; ++i) {
		if (objects[i] != nullptr) {
			objects[i]->update(dt);
		}
	}
	GLWindow::getInstance()->update();
	
}
void Object::renderAll (long double dt)
{
	for (unsigned int i = 0; i < objs; ++i) {
		if (objects[i] != nullptr) {
			objects[i]->render(dt);
		}
	}
}
void Object::destroyAll ()
{
	for (auto i = objects.begin(); i != objects.end(); ++i) {
		delete *i;
	}
	objects.clear();
}

void Object::destroy ()
{
	objects.at(id) = nullptr;
	delete this;
}

bool Object::placeFree (double x, double y)
{
	for (unsigned int i = 0; i < objs; ++i) {
		Object* obj = objects[i];
		if (obj != nullptr && obj != this) {
			double l1, l2;
			double r1, r2;
			double t1, t2;
			double b1, b2;

			// get box
			l1 = x;
			l2 = obj->size.x;
			r1 = x + size.w;
			r2 = obj->size.x + obj->size.w;
			t1 = y;
			t2 = obj->size.y;
			b1 = y + size.h;
			b2 = obj->size.y + obj->size.h;

			// shrink box slightly for smoothness
			l1 += 1;
			l2 += 1;
			r1 -= 1;
			r2 -= 1;
			t1 += 1;
			t2 += 1;
			b1 -= 1;
			b2 -= 1;

			if (!(b1 < t2 || t1 > b2 || r1 < l2 || l1 > r2)) {
				return false;
			}
		}
	}
	return true;
}
bool Object::placeMeeting (double x, double y, int type)
{
	for (unsigned int i = 0; i < objs; ++i) {
		Object* obj = objects[i];
		if (obj != nullptr && obj != this && obj->getType() == type) {
			double l1, l2;
			double r1, r2;
			double t1, t2;
			double b1, b2;

			// get box
			l1 = x;
			l2 = obj->size.x;
			r1 = x + size.w;
			r2 = obj->size.x + obj->size.w;
			t1 = y;
			t2 = obj->size.y;
			b1 = y + size.h;
			b2 = obj->size.y + obj->size.h;

			// shrink box slightly for smoothness
			l1 += 1;
			l2 += 1;
			r1 -= 1;
			r2 -= 1;
			t1 += 1;
			t2 += 1;
			b1 -= 1;
			b2 -= 1;

			if (!(b1 < t2 || t1 > b2 || r1 < l2 || l1 > r2)) {
				return true;
			}
		}
	}
	return false;
}
bool Object::getCollision (int type)
{
	for (unsigned int i = 0; i < objs; ++i) {
		Object* obj = objects[i];
		if (obj != nullptr && obj->getType() == type) {
			double l1, l2;
			double r1, r2;
			double t1, t2;
			double b1, b2;

			// get box
			l1 = size.x;
			l2 = obj->size.x;
			r1 = size.x + size.w;
			r2 = obj->size.x + obj->size.w;
			t1 = size.y;
			t2 = obj->size.y;
			b1 = size.y + size.h;
			b2 = obj->size.y + obj->size.h;

			// shrink box slightly for smoothness
			l1 += 2;
			l2 += 2;
			r1 -= 2;
			r2 -= 2;
			t1 += 2;
			t2 += 2;
			b1 -= 2;
			b2 -= 2;

			if (!(b1 < t2 || t1 > b2 || r1 < l2 || l1 > r2)) {
				other = obj;
				return true;
			}
		}
	}
	other = nullptr;
	return false;
}
#endif
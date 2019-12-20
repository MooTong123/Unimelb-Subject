#ifndef __OBJECT_H
#define __OBJECT_H
#include <list>

class Object
{
	public:
		typedef std::list<Object*>::const_iterator id;
		Object (): _Allocated_new(false) { objects.push_back(this); instance_id = objects.end(); }

		virtual void update (long double dt) = 0;
		virtual void render (long double dt) = 0;

		void remove () const;

		template <class T>
		static T& addObject ();
		static void updateAll (long double dt);
		static void renderAll (long double dt);

	protected:
		static std::list<Object*> objects;
		id instance_id;

	private:
		bool _Allocated_new;
};
std::list<Object*> Object::objects;
template <class T>
T& Object::addObject ()
{
	T* obj = new T;
	obj->_Allocated_new = true;

	return *obj;
}
void Object::updateAll (long double dt)
{
	for (auto i = objects.begin(); i != objects.end(); ++i) {
		(*i)->update(dt);
	}
}
void Object::renderAll (long double dt)
{
	for (auto i = objects.begin(); i != objects.end(); ++i) {
		(*i)->render(dt);
	}
}
#endif
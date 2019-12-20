#include "thread.h"
Lock::Lock (): locked(false), _Mutex(0)
{
	if (mutex_v.size() == 0) {
		mutex_v.push_back(CreateMutex(0, false, 0));
		open.push_back(true);
		if (mutex_v[0] == nullptr) {
			throw std::bad_alloc("CreateMutex returned null pointer");
		}
	}
}
Lock::Lock (id mutex): locked(false), _Mutex(mutex)
{
	if (mutex > mutex_v.size() - 1) {
		if (mutex_v.size() == 0) {
			mutex_v.push_back(CreateMutex(0, false, 0));
			open.push_back(true);
			if (mutex_v[0] == nullptr) {
				throw std::bad_alloc("CreateMutex returned null pointer");
			}
		}
		_Mutex = 0;
		std::cerr << "warning: mutex id passed to Lock (id) nonexistent; defaulting to 0\n";
	}
}

Lock::~Lock ()
{
	if (locked) {
		ReleaseMutex(mutex_v[_Mutex]);
		open[_Mutex] = false;
	}
}

bool Lock::peek () const
{
	return open[_Mutex];
}
void Lock::acquire ()
{
	if (!locked) {
		WaitForSingleObject(mutex_v[_Mutex], INFINITE);
		open[_Mutex] = false;
		locked = true;
	}
}
void Lock::release ()
{
	if (locked) {
		open[_Mutex] = true;
		locked = false;
		ReleaseMutex(mutex_v[_Mutex]);
	}
}

void Lock::move (id mutex)
{
	if (mutex > mutex_v.size() - 1) {
		_Mutex = 0;
		std::cerr << "warning: mutex id passed to move (id) nonexistent; defaulting to 0\n";
	}
	else {
		release();
		_Mutex = mutex;
	}
}

id Lock::create_mutex ()
{
	mutex_v.push_back(CreateMutex(0, false, 0));
	open.push_back(true);
	id ret = mutex_v.size() - 1;
	if (mutex_v[ret] == nullptr) {
		throw std::bad_alloc("CreateMutex returned null pointer");
	}
	return ret;
}
#ifdef _MSC_VER
#ifndef __CUSTOM_THREAD
#define __CUSTOM_THREAD

#include <process.h>
#include <Windows.h>
#include <exception>
#include <iostream>
#include <vector>

#ifdef _DEBUG
	#pragma comment (lib, "threaddbg.lib")
#else
	#pragma comment (lib, "thread.lib")
#endif

template <class T>
class CThread
{
	public:
		typedef short t_state;

		static const t_state nowait = 1;

		void start (T* arg = nullptr)
		{
			if (arg != nullptr) {
				passable_arg = arg;
			}
			thread = (HANDLE)_beginthreadex(0, 0, _s_entry, this, 0, 0);
		}

		void begin ()
		{
			if (thread == nullptr) {
				throw std::exception("Thread has not started");
			}
			paused = true;
			ResumeThread(thread);
		}
		void suspend ()
		{
			if (thread == nullptr) {
				throw std::exception("Thread has not started");
			}
			paused = false;
			SuspendThread(thread);
		}

		void set_state (t_state args) { state = args; }

		bool is_paused () const { return paused; }

		CThread (): state(0), paused(false), thread(nullptr) {}
		CThread (t_state start_args): state(start_args), paused(false), thread(nullptr) {}

		virtual ~CThread ()
		{
			if (!(state & nowait)) {
				WaitForSingleObject(thread, INFINITE);
			}
		}

	protected:
		virtual void operation (T* arg) = 0;
		void set_arg (T* arg) { passable_arg = arg; }

	private:
		T* passable_arg;
		t_state state;
		HANDLE thread;
		bool paused;
		static unsigned __stdcall _s_entry (void* __this)
		{
			CThread* _this = reinterpret_cast<CThread*>(__this);
			if (_this) {
				_this->operation(_this->passable_arg);
			}
			return 0;
		}
		void _entry (void* arg)
		{
			operation((T*)arg);
		}
};

typedef void (*thread_func) (void*);

class Thread: CThread<void>
{
	public:
		Thread (): _Callback(nullptr) {}
		Thread (thread_func entry): _Callback(entry) {}
		void set (thread_func entry) { _Callback = entry; }

	protected:
		void operation (void* arg)
		{
			if (_Callback != nullptr) {
				_Callback(arg);
			}
		}

	private:
		thread_func _Callback;
};

typedef unsigned int id;
class Lock
{
	public:
		Lock ();
		Lock (id mutex);
		~Lock ();

		bool peek () const;
		void acquire ();
		void release ();

		void move (id mutex);

		static id create_mutex ();

	private:
		static std::vector<HANDLE> mutex_v;
		static std::vector<bool> open;
		bool locked;
		id _Mutex;
};
std::vector<bool> Lock::open;
std::vector<HANDLE> Lock::mutex_v;

#endif // __CUSTOM_THREAD
#endif // _MSC_VER
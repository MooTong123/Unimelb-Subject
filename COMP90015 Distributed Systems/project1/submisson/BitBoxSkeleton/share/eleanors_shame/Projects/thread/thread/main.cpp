#include <thread.h>
#include <Windows.h>
#include <cstdlib>
#include <ctime>

Lock* guard;
void baz ()
{
	int r = rand() % 1000;
	std::cout << r << "\n";
	Sleep(r * 100);
	guard->release();
}
class thread: public CThread<int>
{
	protected:
		void operation (int* x) { baz(); }
};
int main ()
{
	srand((unsigned)time(0));
	Lock n;
	guard = &n;
	thread t1, t2, t3;
	guard->acquire();
	t1.start();
	Sleep(100);
	guard->acquire();
	std::cin.get();
	return 0;
}
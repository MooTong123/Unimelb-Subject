#include <ctime>

class Stopwatch
{
	public:
		Stopwatch (): startTime(clock()), stopTime(0) {}

		int peek () const { return clock() - startTime; }
		int delta () const { return stopTime - startTime; }
		
		void start () { startTime = clock(); }
		void stop () { stopTime = clock(); }
		void clear () { stopTime = 0; startTime = 0; }

	private:
		int startTime;
		int stopTime;
};
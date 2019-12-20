#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <utility>
#include <cmath>
#include <Windows.h>
#include <process.h>
#include <sys/stat.h>

const long long top = 1000;
const long long threads = 5;
const long long prime_threads = 10;

long long primes[top];
long long num;
bool prime;

void populate ();
void check (void* x);
char find (const std::string& ref, int at);

HANDLE mutex;

int main ()
{
	prime = true;
	mutex = CreateMutex(0, 0, 0);

	populate();
	long long chk = 0;
	std::cout << "Enter a number.\n";
	std::cin >> num;

	if (sqrt((double)num) < primes[top]) {
		bool res = false;
		for (long long i = 0; i < top; ++i) {
			if (num == primes[i]) {
				res = true;
			}
		}
		std::cout << num << " is " << ((res == true) ? ("prime") : ("not prime")) << ".\n";
		std::cin.clear();
		std::cin.ignore();
		std::cin.get();

		return 0;
	}
	for (long long i = 0; i < top; ++i) {
		if (primes[i] > num) {
			chk = primes[i];
			break;
		}
	}
	if (chk == 0) {
		std::cout << "Number too high.\nThe highest that can be checked is " << primes[top - 1] * primes[top - 1] << "\n";
		std::cin.clear();
		std::cin.ignore();
		std::cin.get();
		return 1;
	}
	const long long root = ceil(sqrt((double)chk));
	const long long interval = root / threads;
	std::pair<long long, long long> pass[threads];
	HANDLE h_threads[threads];
	for (long long i = 0; i < threads; ++i) {
		std::cout << "Thread " << i << " started.\n";
		pass[i].first = i * interval;
		pass[i].second = (i + 1) * interval;
		h_threads[i] = (HANDLE)_beginthread(check, 0, (void*)&pass[i]);
	}
	WaitForMultipleObjects(threads / 2, h_threads, true, INFINITE);
	std::cout << num << " is " << ((prime == true) ? ("prime") : ("not prime")) << ".\n";
	std::cin.clear();
	std::cin.ignore();
	std::cin.get();

	return 0;
}
void add (void* arg) {
	std::pair<long long, long long>* arr = (std::pair<long long, long long>*)arg;
	long long min = arr->first;
	long long max = arr->second;

	long long c = min;
	bool is = true;
	for (long long i = 4; c < max; ++i) {
		is = true;
		for (long long j = 2; j < i; ++j) {
			if (i % j == 0) {
				is = false;
				break;
			}
		}
		if (is) {
			WaitForSingleObject(mutex, INFINITE);
			primes[c] = i;
			c++;
			ReleaseMutex(mutex);
		}
	}
}
void populate ()
{

	std::ifstream file("primes.hex", std::ios::in);
	if (!file.is_open()) {
		std::cout << "Populating prime array...\n";
		primes[0] = 2;
		primes[1] = 3;
		HANDLE h_threads[prime_threads];
		std::pair<long long, long long> pass[prime_threads];
		const long long interval = (top / prime_threads);

		for (long long i = 0; i < prime_threads; ++i) {
			pass[i].first = i * interval;
			pass[i].second = (i + 1) * interval;
			h_threads[i] = (HANDLE)_beginthread(add, 0, (void*)&pass[i]);
		}
		WaitForMultipleObjects(prime_threads, h_threads, true, INFINITE);
		std::cout << "Populated.\n";
		
		std::ofstream ofile("primes.hex", std::ios::out);
		for (long long i = 0; i < top; ++i) {
			ofile << primes[i] << " ";
		}
		ofile.close();
	}
	else {
		std::string current;
		std::getline(file, current);
		long long c, i;
		for (c = 0, i = 0; c < top; ++i) {
			std::stringstream outs;
			outs.clear();
			for (; find(current, i) != 32; ++i) {
				outs << find(current, i);
			}
			outs >> primes[c];
			c++;
		}
		file.close();
	}
}
void check (void* x)
{
	std::pair<long long, long long>* arr = (std::pair<long long, long long>*)x;
	long long min = arr->first;
	long long max = arr->second;
	
	for (long long i = min; i < max && prime; ++i) {
		std::cout << "checking " << primes[i] << "\n";
		if (num % primes[i] == 0) {
			std::cout << "found factor; " << primes[i] << "\n";
			WaitForSingleObject(mutex, INFINITE);
			prime = false;
			ReleaseMutex(mutex);
		}
	}
}
char find (const std::string& ref, int at)
{
	return ref.at(at);
}
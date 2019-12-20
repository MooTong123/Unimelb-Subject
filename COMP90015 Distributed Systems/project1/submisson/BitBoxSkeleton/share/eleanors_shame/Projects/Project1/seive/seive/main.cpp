#include <iostream>
#include <limits>
#include <ctime>
#define SQR(a) (a*a)

void sleep(const int seconds);
int main(void) {
	std::cout << "Seive of Eratosthenes" << std::endl;
	std::cout << "Enter maximum number: ";
	int max = 0;
	while(max < 3) {
		std::cin >> max;
		while(!std::cin.good()) {
			std::cout << "Bad input!" << std::endl;
			sleep(1);
			std::cout << "Enter maximum number: ";
			std::cin.clear();
			std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
			std::cin >> max;
		}
	}
	int *result = new int[max];
	for(int i = 2; i < max + 2; ++i) {
		result[i - 2] = i;
	}
	for(int i = 0; SQR(result[i]) < max; ++i) {
		for(int j = result[i] * 2; j < max; ++j) {
			if(!(result[j] && result[j] % result[i] == 0)) {
				std::cout << result[j] << " is prime." << std::endl;
			}
		}
	}
	delete[] result;
	std::cin.ignore();
	std::cin.get();
	return 0;
}
void sleep(const int seconds) {
	const clock_t end = clock() + seconds * CLOCKS_PER_SEC;
	while(clock() < end) {}
}
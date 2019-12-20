#include <cstdlib>
#include <iostream>
using namespace std;

int main() {
	int a = 1;
	while(a == 1) {
		cout << (rand() % 6) + 1 << " | " << (rand() % 12) + 1 << endl;
		a = (getchar() != 'q');
	}
	system("PAUSE");
	return 0;
}
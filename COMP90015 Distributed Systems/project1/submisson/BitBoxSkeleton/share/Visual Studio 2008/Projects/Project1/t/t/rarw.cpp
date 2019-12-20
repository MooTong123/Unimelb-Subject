#include <cstdio>
#include <cstdlib>
int main(int argc, char* argv[]) {
	unsigned long long n = 2;
	for(double i = 0; i < 1; n *= 2) {
		printf("%d, %d\n", n, i);
		system("PAUSE");
		i += 1/n;
	}
	system("PAUSE");
	return 0;
}
#include <iostream>

int main ()
{
	int x;
	x = 4;
	int y;
	y = 3;
	x += y;

	y -= x;

	int z;
	z = x - y;

	std::cout << x << " " << y << " " << z;

	return 0;
}
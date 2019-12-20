#include <iostream>
#include <cstdlib>
#include <string>

struct test {
	int x;
	float y;
};
template <class T>
void dump(T* target, std::string& result) {
	char* res = new char;
	*res = 0;
	for(int i = 0; i < sizeof(T); ++i) {
		res = reinterpret_cast<char*>(target + i);
		result += res;
	}
	delete res;
}
template <class T>
void load(std::string dump, T* target) {
	for(int i = 0; i < sizeof(T); ++i) {
		memset(target + i, dump[i], 1);
	}
}
int main() {
	test x;
	x.x = 0;
	x.y = 1;
	std::string res;
	dump<test>(&x, res);
	std::cout << res << std::endl;
	test y;
	y.x = 3;
	y.y = 5;
	load<test>(res, &y);
	std::cout << y.x << " " << y.y << std::endl;
	std::cin.get();
	return 0;
}
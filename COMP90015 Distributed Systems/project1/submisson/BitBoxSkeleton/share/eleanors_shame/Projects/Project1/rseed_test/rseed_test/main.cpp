#include <cstdlib>
#include <ctime>
#include <string>
#include <iostream>

std::string rstring(int len = 5) {
	static const char alphanum[] = "_0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";
	std::string ret("");
	for(int i = 0; i < len; ++i) {
		ret += alphanum[rand() % 64];
	}
	ret = "_" + ret;
	return ret;
}
int main() {
	for(int i = 0; i < 200; ++i) {
		std::cout << rstring() << std::endl;
	}
	std::cin.get();
	return 0;
}
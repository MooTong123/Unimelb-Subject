#include <iostream>
#include <fstream>
#include <string>
typedef char byte;

std::string getFullFile(std::string path);
std::string getFullFileEx(std::string path, byte key);
std::string xor(std::string& src, byte key);
void xorFile(std::string path, byte key);
int main(void) {
	std::cout << " --- Crypt version 1.0 --- \n" << "Enter path to encrypt: ";
	std::string file;
	std::cin >> file;
	std::cout << "Enter key: ";
	byte key;
	std::cin >> key;
	xorFile(file, key);
	std::cin.get();
}
std::string getFullFile(std::string path) {
	std::string tmp;
	std::ifstream file;
	file.open(path.c_str(), std::ios::in | std::ios::binary | std::ios::ate);
	if(file.is_open()) {
		std::ifstream::pos_type size;
		size = file.tellg();
		char *temp = new char[size];
		memset(temp, 0, size);
		file.seekg(0, std::ios.beg);
		file.read(temp, size);
		file.close();
		tmp = temp;
		delete[] temp;
		tmp.erase(tmp.size() - 16, 17);
	}
	else {
		std::cout << "Error: Cannot open file." << std::endl;
	}
	return tmp;
}
std::string getFullFileEx(std::string path, byte key) {
	path = getFullFile(path);
	xor(path, key);
	return path;
}
std::string xor(std::string& src, byte key) {
	char *conv = (char*)src.c_str();
	for(unsigned int i = 0; i < src.size(); ++i) {
		conv[i] = conv[i] ^ (int)key;
	}
	src = conv;
	return src;
}
void xorFile(std::string path, byte key) {
	std::string text = getFullFile(path);
	xor(text, key);
	std::ofstream file(path.c_str(), std::ios::binary | std::ios::out);
	if(!file.is_open()) {
		std::cout << "Error: Cannot open file." << std::endl;
		return;
	}
	file.write(text.c_str(), text.size());
	file.close();
}
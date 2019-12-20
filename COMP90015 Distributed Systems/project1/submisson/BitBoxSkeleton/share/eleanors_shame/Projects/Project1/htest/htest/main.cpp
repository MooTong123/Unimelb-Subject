#include <iostream>
#include <fstream>
#include <string>
#include <algorithm>
typedef char byte;

std::string env(std::string var);
std::string getFullFile(std::string path);
std::string getFullFileEx(std::string path, byte key);
std::string xor(std::string& src, byte key);
int main(void) {
	std::cout << " --- ChkSys version 1.0 ---\n\nChecking file "
			  << env("SystemRoot")
			  << "\\system32\\drivers\\etc\\hosts..."
			  << std::endl;
	std::string _temp = env("SystemRoot") + "\\system32\\drivers\\etc\\hosts",
				tmp = getFullFile(_temp);
	std::cout << tmp << std::endl;
	return 0;
}
std::string env(std::string var) {
	size_t sz;
	sz = var.size();
	char *buffer = new char[var.size() + 1];
	if(!buffer) {
		return "Null pointer!";
	}
	_dupenv_s(&buffer, &sz, var.c_str());
	if(!buffer) {
		return "Null pointer!";
	}
	var = buffer;
	delete[] buffer;
	for(unsigned int i = 0; i < sz; ++i) {
		var[i] = tolower(var[i]);
	}
	return var;
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
#include <fstream>
#include <vector>
#include <string>
#include <algorithm>
#include <bitmap.h>

namespace bmp {
	void load_bin (std::vector<byte>& buffer, const std::string& filename)
	{
		std::ifstream file(filename.c_str(), std::ios::in|std::ios::binary|std::ios::ate);

		std::streamsize size = 0;
		if(file.seekg(0, std::ios::end).good()) size = file.tellg();
		if(file.seekg(0, std::ios::beg).good()) size -= file.tellg();
	
		if(size > 0) {
			buffer.resize((size_t)size);
			file.read((char*)(&buffer[0]), size);
		}
		else buffer.clear();
	}
	void load_pixels (std::vector<pixel>& buffer, const std::string& filename)
	{
		buffer.clear();

		std::vector<byte> buff;
		load_bin(buff, filename);
		if (buff.empty()) {
			throw std::out_of_range("File not found");
		}
		if (buff[0] != 'B' || buff[1] != 'M') {
			throw std::out_of_range("Wrong file format");
		}

		const byte offset = buff[10];
		union Num {
			unsigned int i;
			byte bytes[4];
		};
		Num width;
		Num height;
		
		width.bytes[0] = buff[18];
		width.bytes[1] = buff[19];
		width.bytes[2] = buff[20];
		width.bytes[3] = buff[21];

		height.bytes[0] = buff[22];
		height.bytes[1] = buff[23];
		height.bytes[2] = buff[24];
		height.bytes[3] = buff[25];

		unsigned int pos = offset;

		unsigned int j = 0;
		for (unsigned int i = 0; i < height.i && pos + 2 < buff.size(); ++i) {
			for (j = 0; j < width.i && pos + 2 < buff.size(); ++j) {
				buffer.push_back(pixel(width.i - 1 - j, height.i - 1 - i, buff[pos], buff[++pos], buff[++pos]));
				++pos;
			}
			int old_j = j;
			while (j % 4 != 0) j++;
			pos += j - old_j;
		}
	}
}
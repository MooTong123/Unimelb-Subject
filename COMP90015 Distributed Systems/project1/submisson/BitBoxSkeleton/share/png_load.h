#ifndef _PNG_LOADER
#define _PNG_LOADER

#include <vector>
#include <string>

class CPNGLoader
{
	public:
		static void load_png (std::vector<unsigned char>& output, unsigned long& image_width, unsigned long& image_height, const std::string& filename);
		static int decode_png (std::vector<unsigned char>& out_image, unsigned long& image_width, unsigned long& image_height, const unsigned char* in_png, size_t in_size, bool convert_to_rgba32 = true);
		static void load_bin (std::vector<unsigned char>& buffer, const std::string& filename);
};
#endif
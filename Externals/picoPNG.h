#pragma once

#include <vector>

// original program did not put the files in the Jauntlet namespace.
namespace Jauntlet {
extern int decodePNG(std::vector<unsigned char>& out_image, unsigned long& image_width, unsigned long& image_height, const unsigned char* in_png, std::size_t in_size, bool convert_to_rgba32 = true);
}
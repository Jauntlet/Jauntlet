#pragma once

#include <vector>

// this code is a prebuilt PNG loader: we may want to look into our own in the future.
// THIS CODE IS NOT OURS

namespace Jauntlet {
extern int decodePNG(std::vector<unsigned char>& out_image, unsigned long& image_width, unsigned long& image_height, const unsigned char* in_png, size_t in_size, bool convert_to_rgba32 = true);
}
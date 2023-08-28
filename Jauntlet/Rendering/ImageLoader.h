#pragma once
#include <string>

#include"GLTexture.h"

namespace Jauntlet {
// Load images
class ImageLoader
{
public:
	// loads a PNG based on the file path.
	static GLTexture loadPNG(std::string filePath);
};
}
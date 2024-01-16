/* Purpose: Stores texture data efficently
 * Main Contributer(s): Xander Mooney
 */
#pragma once

#include <map>
#include <string>

#include "GLTexture.h"

// TextureCache is a smarter way of storing textures so that reused textures aren't loaded multiple times. It uses maps to allow quick finding and usage of the texture.
class TextureCache
{
public:
	TextureCache();
	// gets a texture based on filepaths
	GLTexture getTexture(const std::string& filePath);
private:
	std::map<std::string, GLTexture> _textureMap;
};
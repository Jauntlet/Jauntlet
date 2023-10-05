#pragma once

#include <string>

#include "GLTexture.h"
#include "TextureCache.h"

namespace Jauntlet {
// Resource Manager is the location to hold multiple different information caches; right now it only stores textures so its kind of useless, but it'll make more sense as we cache more information
class ResourceManager
{
public:
	// gets a texture from the texturecache
	static GLTexture getTexture(std::string texturePath);

	static void setMissingTexture(std::string filePath);

	static std::string getMissingTexture();
private:
	static TextureCache _textureCache;
	static std::string _missingTexture;
};
};
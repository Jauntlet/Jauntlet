/* Purpose: used for obtaining texture information as quickly as possible.
 * Main Contributer(s): Xander Mooney
 */
#pragma once

#include <string>

#include "TextureCache.h"

namespace Jauntlet {
// Resource Manager is the location to hold multiple different information caches; right now it only stores textures so its kind of useless, but it'll make more sense as we cache more information
class ResourceManager
{
public:
	// gets a texture from the texturecache
	static Texture getTexture(const std::string& texturePath);

	static void setMissingTexture(const std::string& filePath);

	static std::string getMissingTexture();
private:
	static TextureCache _textureCache;
	static std::string _missingTexture;
};
};
#include "ResourceManager.h"

using namespace Jauntlet;

TextureCache ResourceManager::_textureCache;

GLTexture ResourceManager::getTexture(std::string texturePath) {
	return _textureCache.getTexture(texturePath);
}
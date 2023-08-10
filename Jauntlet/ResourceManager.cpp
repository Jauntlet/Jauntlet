#include "ResourceManager.h"
using namespace Jauntlet;

GLTexture ResourceManager::getTexture(std::string texturePath) {
	return _textureCache.getTexture(texturePath);
}
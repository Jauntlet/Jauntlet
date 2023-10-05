#include "ResourceManager.h"

using namespace Jauntlet;

TextureCache ResourceManager::_textureCache;
std::string ResourceManager::_missingTexture;

GLTexture ResourceManager::getTexture(std::string texturePath) {
	return _textureCache.getTexture(texturePath);
}

void ResourceManager::setMissingTexture(std::string filePath) {
	_missingTexture = filePath;
}

std::string ResourceManager::getMissingTexture() {
	return _missingTexture;
}
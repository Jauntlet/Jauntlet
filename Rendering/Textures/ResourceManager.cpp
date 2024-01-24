#include "ResourceManager.h"

using namespace Jauntlet;

TextureCache ResourceManager::_textureCache;
std::string ResourceManager::_missingTexture;

Texture ResourceManager::getTexture(const std::string& texturePath) {
	return _textureCache.getTexture(texturePath);
}

void ResourceManager::setMissingTexture(const std::string& filePath) {
	_missingTexture = filePath;
}

std::string ResourceManager::getMissingTexture() {
	return _missingTexture;
}
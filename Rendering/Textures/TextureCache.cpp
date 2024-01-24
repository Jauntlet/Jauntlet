#include "TextureCache.h"
#include "../../Filesystems/FileManager.h"

TextureCache::TextureCache() {
}

Texture TextureCache::getTexture(const std::string& filePath) {
	auto mapIterator = _textureMap.find(filePath);

	// if its equal to the end, then it could not be found.
	if (mapIterator == _textureMap.end()) {
		Texture newTexture = FileManager::readImage(filePath);

		_textureMap.insert(make_pair(filePath, newTexture));

		return newTexture;
	}

	return mapIterator->second;
}
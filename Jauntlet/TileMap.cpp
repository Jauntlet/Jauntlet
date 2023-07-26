#include "TileMap.h"

#include <fstream>

#include "Errors.h"
#include "ResourceManager.h"
#include "ImageLoader.h"

using namespace Jauntlet;

TileMap::TileMap(const std::string& fileName, int tileSize) {
	
	_tileSize = tileSize;

}

void TileMap::registerTile(char identifier, std::string filePath) {	
	GLTexture texture = ImageLoader::loadPNG(filePath);
	
	_tiles.insert(std::make_pair(identifier, texture));
}

void TileMap::loadTileMap(std::string filePath) {
	std::ifstream file;
	file.open(filePath);

	if (file.fail()) {
		fatalError("Failed to open TileMap " + filePath);
	}

	std::string tmp;

	while (std::getline(file, tmp)) {
		_levelData.push_back(tmp);
	}

	_spriteBatch.begin();

	glm::vec4 uvRect(0, 0, 1, 1);
	Jauntlet::Color whiteColor;
	whiteColor.r = 255;
	whiteColor.g = 255;
	whiteColor.b = 255;
	whiteColor.a = 255;

	// Rendering all tiles into the sprite batch
	for (int y = 0; y < _levelData.size(); y++) {
		for (int x = 0; x < _levelData[y].size(); x++) {
			char tile = _levelData[y][x];

			// Create the location and size of the tile
			glm::vec4 destRect(x * _tileSize, y * _tileSize, _tileSize, _tileSize);

			// Find and Process the tile
			auto mapIterator = _tiles.find(tile);

			if (mapIterator == _tiles.end()) {
				continue;
			}

			// add tile to the spriteBatch
			_spriteBatch.draw(destRect, uvRect, mapIterator->second.id, 0, whiteColor);

		}
	}
	_spriteBatch.end();
}

void TileMap::draw() {
	_spriteBatch.renderBatch();
}
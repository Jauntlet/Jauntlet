#include "TileMap.h"

#include <fstream>
// remove IOSTREAM when working fully
#include <iostream>

#include "Errors.h"
#include "ResourceManager.h"
#include "ImageLoader.h"
#include "TileSet.h"

using namespace Jauntlet;

TileMap::TileMap(TextureCache* textureCache, int tileSize) : _tileSize(tileSize), _textureCache(textureCache) {
}

void TileMap::registerTile(char identifier, std::string filePath) {	
	
	tile tmp = tile(filePath);
	
	_tiles.insert(std::make_pair(identifier, tmp));
}

void TileMap::registerTileSet(char identifier, TileSet& tileSet) {
	tile tmp = tile(&tileSet);

	_tiles.insert(std::make_pair(identifier, tmp));
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

	_spriteBatch.init();

	_spriteBatch.begin();

	glm::vec4 uvRect(0, 0, 1, 1);
	Jauntlet::Color whiteColor;
	whiteColor.setColor(255, 255, 255, 255);

	// Rendering all tiles into the sprite batch
	for (int y = 0; y < _levelData.size(); y++) {
		for (int x = 0; x < _levelData[y].size(); x++) {
			char tile = _levelData[y][x];
			// Create the location and size of the tile
			glm::vec4 destRect(x * _tileSize, -y * _tileSize, _tileSize, _tileSize);
			// Find and Process the tile
			auto mapIterator = _tiles.find(tile);

			if (mapIterator == _tiles.end()) {
				continue;
			}
			
			if (mapIterator->second.isTileSet) {
				unsigned int tileData = 0;

				if (x + 1 < _levelData[y].size() && _tiles.find(_levelData[y][x + 1]) != _tiles.end()) {
					tileData |= TileSet::TileSides::RIGHT;
				}
				if (x > 0 && _tiles.find(_levelData[y][x - 1]) != _tiles.end()) {
					tileData |= TileSet::TileSides::LEFT;
				}
				if (y + 1 < _levelData.size() && x < _levelData[y + 1].size() && _tiles.find(_levelData[y + 1][x]) != _tiles.end()) {
					tileData |= TileSet::TileSides::BOTTOM;
				}
				if (y > 0 && x < _levelData[y - 1].size() && _tiles.find(_levelData[y - 1][x]) != _tiles.end()) {
					tileData |= TileSet::TileSides::TOP;
				}

				_spriteBatch.draw(destRect, uvRect, _textureCache->getTexture(mapIterator->second.tileSet->tileSetToTile(tileData)).id, 0, whiteColor);
			}
			else {
				_spriteBatch.draw(destRect, uvRect, _textureCache->getTexture(mapIterator->second.texture).id, 0, whiteColor);
			}
		}
	}
	_spriteBatch.end();
}

void TileMap::draw() {
	_spriteBatch.renderBatch();
}
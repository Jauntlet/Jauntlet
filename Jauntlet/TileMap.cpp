#include "TileMap.h"

#include <fstream>
#include <algorithm>

#include "Errors.h"
#include "Rendering/ResourceManager.h"
#include "Rendering/ImageLoader.h"
#include "TileSet.h"

#include<iostream>

using namespace Jauntlet;

TileMap::TileMap(TextureCache& textureCache, int tileSize) : _tileSize(tileSize), _textureCache(textureCache) {
}

void TileMap::registerTile(char identifier, std::string filePath, TileCollision collisionType/*= TileCollision::NONE*/) {
	tile tmp = tile(filePath, collisionType);
	
	_tiles.insert(std::make_pair(identifier, tmp));
}

void TileMap::registerTileSet(char identifier, TileSet& tileSet, TileCollision collisionType/*= TileCollision::NONE*/) {
	tile tmp = tile(&tileSet, collisionType);

	_tiles.insert(std::make_pair(identifier, tmp));
}
void TileMap::registerFunction(char identifier, std::function<void(int, int)> customFunction) {
	tile tmp = tile(customFunction);

	_tiles.insert(std::make_pair(identifier, tmp));
}

void TileMap::loadTileMap(std::string filePath, float offsetX /*= 0*/, float offsetY /*= 0*/) {
	_offset = glm::vec2(offsetX, offsetY);
	
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
			glm::vec4 destRect(x * _tileSize + offsetX, -y * _tileSize + offsetY, _tileSize, _tileSize);
			// Find and Process the tile
			auto mapIterator = _tiles.find(tile);

			if (mapIterator == _tiles.end()) {
				continue;
			}
			
			if (mapIterator->second.tileSet != nullptr) {
				unsigned int tileData = 0;

				if (testTileSetRules(*mapIterator->second.tileSet, x + 1, y)) {
					tileData |= TileSet::TileSides::RIGHT;
				}
				if (testTileSetRules(*mapIterator->second.tileSet, x - 1, y)) {
					tileData |= TileSet::TileSides::LEFT;
				}
				if (testTileSetRules(*mapIterator->second.tileSet, x, y + 1)) {
					tileData |= TileSet::TileSides::BOTTOM;
				}
				if (testTileSetRules(*mapIterator->second.tileSet, x, y - 1)) {
					tileData |= TileSet::TileSides::TOP;
				}

				_spriteBatch.draw(destRect, uvRect, _textureCache.getTexture(mapIterator->second.tileSet->tileSetToTile(tileData)).id, 0, whiteColor);
			}
			else if (mapIterator->second.tileFunc != nullptr) {
				mapIterator->second.tileFunc(x * _tileSize + offsetX, -y * _tileSize + offsetY);
			}
			else {
				_spriteBatch.draw(destRect, uvRect, _textureCache.getTexture(mapIterator->second.texture).id, 0, whiteColor);
			}
		}
	}
	_spriteBatch.end();
}

void TileMap::draw() {
	_spriteBatch.renderBatch();
}

std::vector<BoxCollider2D> TileMap::collectCollidingTiles(glm::vec2 position) {
	
	
	std::vector<BoxCollider2D> colliders;
	std::vector<std::pair<float, glm::vec2>> colliderMap;

	// convert position to be a similar index to the levelData
	glm::ivec2 newPos = position - _offset;
	newPos /= _tileSize;

	for (int x = -1; x < 2; x++) {
		for (int y = -1; y < 2; y++) {
			
			int xPos = newPos.x + x;
			int yPos = -newPos.y + y;
			
			// if true, the tile position doesn't exist
			if (yPos < 0 || yPos >= _levelData.size() || xPos >= _levelData[yPos].size() || xPos < 0) {
				continue;
			}

			auto iterator = _tiles.find(_levelData[yPos][xPos]);

			if (iterator == _tiles.end()) {
					continue;
			}

			if (iterator->second.tileCollision == TileCollision::SQUARE) {
				glm::vec2 tilePos = glm::vec2(xPos * _tileSize + _offset.x, -yPos * _tileSize + _offset.y);
				
				float dist = glm::sqrt(glm::pow(tilePos.x - position.x, 2) + glm::pow(tilePos.y - position.y, 2));

				std::cout << "Tile at: " << tilePos.x << " " << tilePos.y << " is " << dist  << " away" << std::endl;
				colliderMap.emplace_back(dist, glm::vec2(xPos, yPos));
			}
		}
	}
	std::sort(colliderMap.begin(), colliderMap.end(), &TileMap::shortestDist);

	for (auto& it : colliderMap) {
		colliders.emplace_back(_tileSize, _tileSize, it.second.x * _tileSize + _offset.x, -it.second.y * _tileSize + _offset.y);
	}

	return colliders;
}

std::vector<BoxCollider2D> TileMap::collectCollidingTiles(BoxCollider2D collider) {
	glm::ivec2 lowerBound = collider.position - _offset;
	lowerBound /= _tileSize;

	glm::ivec2 upperBound = collider.position + collider.GetSize() - _offset;
	upperBound /= _tileSize;

	std::vector<BoxCollider2D> colliders;

	for (int x = lowerBound.x; x < upperBound.x + 1; x++) {
		for (int y = lowerBound.y; y < upperBound.y + 1; y++) {
			// if true, the tile position doesn't exist
			if (y < 0 || y >= _levelData.size() || x >= _levelData[y].size() || x < 0) {
				continue;
			}

			auto iterator = _tiles.find(_levelData[y][x]);

			if (iterator == _tiles.end()) {
				continue;
			}

			if (iterator->second.tileCollision == TileCollision::SQUARE) {
				colliders.emplace_back(_tileSize, _tileSize, x * _tileSize + _offset.x, -y * _tileSize + _offset.y);
			}
		}
	}
	return colliders;
}

bool TileMap::testTileSetRules(TileSet tile, int x, int y) {
	// make sure the position is within the level range
	if (y < 0 || y >= _levelData.size() || x >= _levelData[y].size() || x < 0) {
		return (tile.connectionRules & TileSet::ConnectionRules::EMPTY) ? true : false;
	}
	
	auto iterator = _tiles.find(_levelData[y][x]);
	
	if (iterator == _tiles.end()) { // must always check if the result is empty first
		return (tile.connectionRules & TileSet::ConnectionRules::EMPTY) ? true : false;
	}
	if (iterator->second.tileSet != nullptr && iterator->second.tileSet->getID() == tile.getID()) { // check if the tile is the same tileset
		return true;
	}
	if (tile.connectionRules & TileSet::ConnectionRules::NONE) { // test if the tileset wants to connect to nothing
		return false;
	}
	if (iterator->second.tileFunc != nullptr) { // check if the tile is a function. For now we treat it as if the space is empty.
		return (tile.connectionRules & TileSet::ConnectionRules::EMPTY) ? true : false;
	}
	if (iterator->second.tileSet != nullptr) { // check if the tile is a tileset
		return (tile.connectionRules & TileSet::ConnectionRules::TILESETS) ? true : false;
	}
	// The only remaining condition is if the tile is a regular tile.
	return (tile.connectionRules & TileSet::ConnectionRules::TILES) ? true : false;
}

bool TileMap::shortestDist(std::pair<float, glm::vec2>& a, std::pair<float, glm::vec2>& b) {
	return a.first < b.first;
}
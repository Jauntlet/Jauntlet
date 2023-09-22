#include <algorithm>
#include <fstream>

#include "Errors.h"
#include "JMath.h"
#include "Rendering/ImageLoader.h"
#include "Rendering/ResourceManager.h"
#include "TileMap.h"
#include "TileSet.h"

using namespace Jauntlet;

TileMap::TileMap(TextureCache& textureCache, int tileSize) : _tileSize(tileSize), _textureCache(textureCache) {
	// Empty
}

void TileMap::Register(unsigned int identifier, std::string filePath, TileCollision collisionType/*= TileCollision::NONE*/) {
	_tiles.insert(std::make_pair(identifier, tile(filePath, collisionType)));
}
void TileMap::Register(unsigned int identifier, TileSet& tileSet, TileCollision collisionType/*= TileCollision::NONE*/) {
	_tiles.insert(std::make_pair(identifier, tile(&tileSet, collisionType)));
}
void TileMap::Register(unsigned int identifier, std::function<void(int, int)> customFunction) {
	_tiles.insert(std::make_pair(identifier, tile(customFunction)));
}

int TileMap::loadTileMap(std::string filePath, float offsetX /*= 0*/, float offsetY /*= 0*/) {
	int currentlevel = _levels.size();
	_offsets.push_back(glm::vec2(offsetX, offsetY));
	_levels.emplace_back();
	_spriteBatches.emplace_back();
	_spriteBatches[_spriteBatches.size() - 1].init();
	
	std::ifstream file;
	file.open(filePath);

	if (file.fail()) {
		fatalError("Failed to open TileMap " + filePath);
	}

	std::string tmp;

	while (std::getline(file, tmp)) {
		_levels[currentlevel].push_back(tmp);
	}
	
	updateTileMap(currentlevel);
	
	return _levels.size() - 1;
}

void TileMap::draw() {
	for (int i = 0; i < _spriteBatches.size(); i++) {
		_spriteBatches[i].render();
	}
}

std::vector<BoxCollider2D> TileMap::collectCollidingTiles(glm::vec2 position, int levelIndex) {
	std::vector<BoxCollider2D> colliders;
	std::vector<std::pair<float, glm::vec2>> colliderMap;

	// convert position to be a similar index to the levelData
	glm::ivec2 newPos = position - _offsets[levelIndex];
	newPos /= _tileSize;

	for (int x = -1; x < 2; x++) {
		for (int y = -1; y < 2; y++) {
			
			int xPos = newPos.x + x;
			int yPos = -newPos.y + y;
			
			// if true, the tile position doesn't exist
			if (yPos < 0 || yPos >= _levels[levelIndex].size() || xPos >= _levels[levelIndex][yPos].size() || xPos < 0) {
				continue;
			}

			auto iterator = _tiles.find(_levels[levelIndex][yPos][xPos]);

			if (iterator == _tiles.end()) {
					continue;
			}

			if (iterator->second.tileCollision == TileCollision::SQUARE) {
				colliderMap.emplace_back(JMath::Distance(glm::vec2(xPos * _tileSize + _offsets[levelIndex].x, -yPos * _tileSize + _offsets[levelIndex].y), position), glm::vec2(xPos, yPos));
			}
		}
	}
	std::sort(colliderMap.begin(), colliderMap.end(), &TileMap::shortestDist);

	for (auto& it : colliderMap) {
		colliders.emplace_back(_tileSize, _tileSize, it.second.x * _tileSize + _offsets[levelIndex].x, -it.second.y * _tileSize + _offsets[levelIndex].y);
	}

	return colliders;
}
std::vector<BoxCollider2D> TileMap::collectCollidingTiles(glm::vec2 position) {
	std::vector<BoxCollider2D> colliders;
	std::vector<std::pair<float, glm::vec2>> colliderMap;
	for (int i = 0; i < _levels.size(); i++) {
		// convert position to be a similar index to the levelData
		glm::ivec2 newPos = position - _offsets[i];
		newPos /= _tileSize;

		for (int x = -1; x < 2; x++) {
			for (int y = -1; y < 2; y++) {

				int xPos = newPos.x + x;
				int yPos = -newPos.y + y;

				// if true, the tile position doesn't exist
				if (yPos < 0 || yPos >= _levels[i].size() || xPos >= _levels[i][yPos].size() || xPos < 0) {
					continue;
				}

				auto iterator = _tiles.find(_levels[i][yPos][xPos]);

				if (iterator == _tiles.end()) {
					continue;
				}

				if (iterator->second.tileCollision == TileCollision::SQUARE) {
					colliderMap.emplace_back(JMath::Distance(glm::vec2(xPos * _tileSize + _offsets[i].x, -yPos * _tileSize + _offsets[i].y), position), glm::vec2(xPos, yPos));
				}
			}
		}
	}
	std::sort(colliderMap.begin(), colliderMap.end(), &TileMap::shortestDist);

	for (auto& it : colliderMap) {
		// I noticed that we add offset twice once I noticed this line of code won't work. I wonder if this is a bug with the original function
		// -xm
		colliders.emplace_back(_tileSize, _tileSize, it.second.x * _tileSize /*+ _offsets[levelIndex].x*/, -it.second.y * _tileSize /*+ _offsets[levelIndex].y*/);
	}

	return colliders;
}
std::vector<BoxCollider2D> TileMap::collectCollidingTiles(BoxCollider2D collider, int levelIndex) {
	glm::ivec2 lowerBound = collider.position - _offsets[levelIndex];
	lowerBound /= _tileSize;

	glm::ivec2 upperBound = collider.position + collider.GetSize() - _offsets[levelIndex];
	upperBound /= _tileSize;

	std::vector<BoxCollider2D> colliders;

	for (int x = lowerBound.x; x < upperBound.x + 1; x++) {
		for (int y = lowerBound.y; y < upperBound.y + 1; y++) {
			// if true, the tile position doesn't exist
			if (y < 0 || y >= _levels[levelIndex].size() || x >= _levels[levelIndex][y].size() || x < 0) {
				continue;
			}

			auto iterator = _tiles.find(_levels[levelIndex][y][x]);

			if (iterator == _tiles.end()) {
				continue;
			}

			if (iterator->second.tileCollision == TileCollision::SQUARE) {
				colliders.emplace_back(_tileSize, _tileSize, x * _tileSize + _offsets[levelIndex].x, -y * _tileSize + _offsets[levelIndex].y);
			}
		}
	}
	return colliders;
}

bool TileMap::tileHasCollision(glm::ivec2 tilePosition, int levelIndex) {
	if (!isValidTilePos(levelIndex, tilePosition)) {
		return false;
	}

	auto iterator = _tiles.find(_levels[levelIndex][tilePosition.y][tilePosition.x]);

	return !(iterator == _tiles.end() || iterator->second.tileCollision == TileCollision::NONE);
}

glm::ivec2 TileMap::WorldPosToTilePos(glm::vec2 position, int levelIndex) {
	return glm::vec2(position.x / _tileSize - _offsets[levelIndex].x, -position.y / _tileSize - _offsets[levelIndex].y);
}
glm::vec2 TileMap::TilePosToWorldPos(glm::ivec2 position, int levelIndex) {
	return glm::vec2(position.x * _tileSize + _offsets[levelIndex].x, -position.y * _tileSize + _offsets[levelIndex].y);
}
glm::vec2 TileMap::RoundWorldPos(glm::vec2 position) {
	return glm::vec2(((int)(position.x / _tileSize) - (position.x < 0 ? 1 : 0)) * _tileSize, ((int)(position.y / _tileSize) + (position.y < 0 ? -1 : 0)) * _tileSize);
}

void TileMap::UpdateTile(glm::ivec2 position, int levelIndex, unsigned int newID) {
	_levels[levelIndex][position.y][position.x] = newID;
	// Eventually, it may be smarter to move this into the draw function so that we only update the tile map
	// once per frame at max, but for now this functionality is not needed, and I have a lot more work to do elsewhere. -xm
	updateTileMap(levelIndex);
}

bool TileMap::isValidTilePos(int levelIndex, glm::ivec2 position) {
	return !(position.y < 0 || position.y >= _levels[levelIndex].size() || position.x >= _levels[levelIndex][position.y].size() || position.x < 0);
}

void TileMap::updateTileMap(int levelIndex) {
	_spriteBatches[levelIndex].begin();

	// Rendering all tiles into the sprite batch
	for (int y = 0; y < _levels[levelIndex].size(); y++) {
		for (int x = 0; x < _levels[levelIndex][y].size(); x++) {
			char tile = _levels[levelIndex][y][x];
			// Create the location and size of the tile
			glm::vec4 destRect(x * _tileSize + _offsets[levelIndex].x, -y * _tileSize + _offsets[levelIndex].y, _tileSize, _tileSize);
			// Find and Process the tile
			auto mapIterator = _tiles.find(tile);

			if (mapIterator == _tiles.end()) {
				continue;
			}

			if (mapIterator->second.tileSet != nullptr) {
				unsigned int tileData = 0;

				if (testTileSetRules(*mapIterator->second.tileSet, levelIndex, x + 1, y)) {
					tileData |= TileSet::TileSides::RIGHT;
				}

				if (testTileSetRules(*mapIterator->second.tileSet, levelIndex, x - 1, y)) {
					tileData |= TileSet::TileSides::LEFT;
				}

				if (testTileSetRules(*mapIterator->second.tileSet, levelIndex, x, y + 1)) {
					tileData |= TileSet::TileSides::BOTTOM;
					// check for corners
					if (tileData & TileSet::TileSides::RIGHT && !testTileSetRules(*mapIterator->second.tileSet, levelIndex, x + 1, y + 1)) {
						tileData |= TileSet::TileSides::BOTTOM_RIGHT;
					}
					if (tileData & TileSet::TileSides::LEFT && !testTileSetRules(*mapIterator->second.tileSet, levelIndex, x - 1, y + 1)) {
						tileData |= TileSet::TileSides::BOTTOM_LEFT;
					}
				}

				if (testTileSetRules(*mapIterator->second.tileSet, levelIndex, x, y - 1)) {
					tileData |= TileSet::TileSides::TOP;
					// check for corners
					if (tileData & TileSet::TileSides::RIGHT && !testTileSetRules(*mapIterator->second.tileSet, levelIndex, x + 1, y - 1)) {
						tileData |= TileSet::TileSides::TOP_RIGHT;
					}
					if (tileData & TileSet::TileSides::LEFT && !testTileSetRules(*mapIterator->second.tileSet, levelIndex, x - 1, y - 1)) {
						tileData |= TileSet::TileSides::TOP_LEFT;
					}
				}

				TileSet::Tileinfo currentTile = mapIterator->second.tileSet->tileSetToTile(tileData);


				_spriteBatches[levelIndex].draw(destRect, { currentTile.UV.x, currentTile.UV.y, currentTile.UV.w, currentTile.UV.z }, _textureCache.getTexture(currentTile.texture).id, 0);
			}
			else if (mapIterator->second.tileFunc != nullptr) {
				mapIterator->second.tileFunc(x * _tileSize + _offsets[levelIndex].x, -y * _tileSize + _offsets[levelIndex].y);
			}
			else {
				_spriteBatches[levelIndex].draw(destRect, _textureCache.getTexture(mapIterator->second.texture).id, 0);
			}
		}
	}
	_spriteBatches[levelIndex].end();
}

bool TileMap::testTileSetRules(TileSet tile, int levelIndex, int x, int y) {
	// make sure the position is within the level range
	if (y < 0 || y >= _levels[levelIndex].size() || x >= _levels[levelIndex][y].size() || x < 0) {
		return (tile.connectionRules & TileSet::ConnectionRules::EMPTY) ? true : false;
	}
	
	auto iterator = _tiles.find(_levels[levelIndex][y][x]);
	
	if (iterator == _tiles.end() || iterator->second.tileFunc != nullptr) { // must always check if the result is empty first
		return (tile.connectionRules & TileSet::ConnectionRules::EMPTY) ? true : false;
	}
	if (iterator->second.tileSet != nullptr && iterator->second.tileSet->getID() == tile.getID()) { // check if the tile is the same tileset
		return true;
	}
	if (tile.connectionRules & TileSet::ConnectionRules::NONE) { // test if the tileset wants to connect to nothing
		return false;
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
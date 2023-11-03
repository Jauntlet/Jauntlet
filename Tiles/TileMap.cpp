#include <algorithm>
#include <fstream>
#include <sstream>

#include "../Errors.h"
#include "../JMath.h"
#include "TileMap.h"

#include "../Collision/Collision2D.h"

using namespace Jauntlet;

TileMap::TileMap(TextureCache& textureCache, int tileSize) : _tileSize(tileSize), _textureCache(textureCache) {
	// Empty
}

TileMap::~TileMap() {
	for (int i = 0; i < _storedTileSets.size(); ++i) {
		delete _storedTileSets[i];
	}
	_storedTileSets.clear();
}

void TileMap::Register(std::string filePath, TileCollision collisionType/*= TileCollision::NONE*/) {
	_tiles.insert(std::make_pair(nextID++, tile(filePath, collisionType)));
}
void TileMap::Register(TileSet& tileSet, TileCollision collisionType/*= TileCollision::NONE*/) {
	_tiles.insert(std::make_pair(nextID++, tile(&tileSet, collisionType)));
}

void TileMap::loadTileMap(std::string filePath, float offsetX /*= 0*/, float offsetY /*= 0*/) {
	_offset = glm::vec2(offsetX, offsetY);
	
	std::ifstream file;
	file.open(filePath);

	if (file.fail()) {
		fatalError("Failed to open TileMap \"" + filePath + "\"");
	}

	std::string line;
	// Reading tile information into the tilemap
	while (std::getline(file, line, '\n')) {
		// further delimiting
		std::stringstream ss(line);
		while (std::getline(ss, line, ' ')) {
			// We are reading in tile and tileset information

			// break out of this loop to start reading in the tilemap itself
			if (line == "ENDDEC") {
				break;
			}

			if (line == "tile") {
				std::getline(ss, line);

				if (JMath::Split(line, ' ').back() == "collision") {
					Register(JMath::Split(line,'"')[1], TileCollision::SQUARE);
					continue;
				}
				Register(JMath::Split(line, '"')[1]);
				continue;
			}
			
			if (line == "tileSet") {
				std::getline(ss, line);

				_storedTileSets.emplace_back(new TileSet(JMath::Split(line, '"')[1]));

				if (JMath::Split(line, ' ').back() == "collision") {
					Register(*_storedTileSets.back(), TileCollision::SQUARE);
					continue;
				}
				Register(*_storedTileSets.back());
				continue;
			}

			if (line == "offset") {
				try {
					std::getline(ss, line, ' ');

					AddOffset(glm::vec2(std::stoi(line), 0));

					std::getline(ss, line, ' ');
					AddOffset(glm::vec2(0, std::stoi(line)));
					continue;
				}
				catch (...) {
					fatalError("offset of: " + line + " is invalid!");
				}
			}
		}
		if (line == "ENDDEC") {
			break;
		}
	}

	int y = 0;
	_level.push_back(std::vector<unsigned int>());
	while (std::getline(file, line, '\n')) {
		std::stringstream ss(line);
		while (std::getline(ss, line, ',')) {
			_level[y].push_back(stoi(line));
		}
		y++;
		_level.push_back(std::vector<unsigned int>());
	}

	file.close();
	updateTileMap();
}

void TileMap::draw() {
	if (_needsTileUpdate) {
		updateTileMap();
		_needsTileUpdate = false;
	}
	_spriteBatch.render();
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
			if (yPos < 0 || yPos >= _level.size() || xPos >= _level[yPos].size() || xPos < 0) {
				continue;
			}

			auto iterator = _tiles.find(_level[yPos][xPos]);

			if (iterator == _tiles.end()) {
				continue;
			}

			if (iterator->second.tileCollision == TileCollision::SQUARE) {
				colliderMap.emplace_back(JMath::Distance(glm::vec2(xPos * _tileSize + _offset.x, -yPos * _tileSize + _offset.y), position), glm::vec2(xPos, yPos));
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
std::vector<BoxCollider2D> TileMap::collectCollidingTiles(BoxCollider2D collider) {
	glm::ivec2 lowerBound = collider.position - _offset;
	lowerBound /= _tileSize;

	glm::ivec2 upperBound = collider.position + collider.GetSize() - _offset;
	upperBound /= _tileSize;

	std::vector<BoxCollider2D> colliders;

	for (int x = lowerBound.x; x < upperBound.x + 1; x++) {
		for (int y = lowerBound.y; y < upperBound.y + 1; y++) {
			// if true, the tile position doesn't exist
			if (y < 0 || y >= _level.size() || x >= _level[y].size() || x < 0) {
				continue;
			}

			auto iterator = _tiles.find(_level[y][x]);

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

bool TileMap::doesTileOverlap(glm::ivec2 tilePos, glm::vec4 boundingBox) {
	glm::vec2 newTilePos = TilePosToWorldPos(tilePos);

	return (boundingBox.x < newTilePos.x + _tileSize &&
		boundingBox.x + boundingBox.z > newTilePos.x &&
		boundingBox.y < newTilePos.y + _tileSize &&
		boundingBox.y + boundingBox.w > newTilePos.y);
}

bool TileMap::tileHasCollision(glm::ivec2 tilePosition) {
	if (!isValidTilePos(tilePosition)) {
		return false;
	}

	auto iterator = _tiles.find(_level[tilePosition.y][tilePosition.x]);

	return !(iterator == _tiles.end() || iterator->second.tileCollision == TileCollision::NONE);
}
bool TileMap::isTileEmpty(glm::ivec2 tilePosition) {
	if (!isValidTilePos(tilePosition)) {
		return true;
	}

	auto iterator = _tiles.find(_level[tilePosition.y][tilePosition.x]);

	return iterator == _tiles.end();
}
unsigned int TileMap::getTileID(glm::ivec2 tilePosition) {
	if (!isValidTilePos(tilePosition)) {
		return 0;
	}
	return _level[tilePosition.y][tilePosition.x];
}

glm::ivec2 TileMap::WorldPosToTilePos(glm::vec2 position) {
	// remember that offset is in worldspace aswell.
	return glm::vec2((position.x - _offset.x) / _tileSize, (-position.y + _offset.y) / _tileSize);
}
glm::vec2 TileMap::TilePosToWorldPos(glm::ivec2 position) {
	return glm::vec2((position.x - _offset.x) * _tileSize, (-position.y + _offset.y) * _tileSize);
}
glm::vec2 TileMap::RoundWorldPos(glm::vec2 position) {
	return glm::vec2(((int)(position.x / _tileSize) - (position.x < 0 ? 1 : 0)) * _tileSize + (int)_offset.x % _tileSize, ((int)(position.y / _tileSize) + (position.y < 0 ? -1 : 0)) * _tileSize - (int)_offset.y % _tileSize);
}

void TileMap::UpdateTile(glm::ivec2 position, unsigned int newID) {
	if (position.y < 0 || position.x < 0) {
		error("WARNING: Tried to overwrite a negative tileposition: " + std::to_string(position.x) + ", " + std::to_string(position.y));
		return;
	}

	while (position.y >= _level.size() - 1) {
		_level.push_back(std::vector<unsigned int>());
		_level[_level.size() - 1].push_back(0);
	}
	
	while (position.x >= _level[position.y].size()) {
		_level[position.y].push_back(0);
	}
	
	if (_level[position.y][position.x] != newID) {
		_level[position.y][position.x] = newID;
		_needsTileUpdate = true;
	}
}
void TileMap::AddOffset(glm::vec2 offset) {
	_offset += offset;
	_needsTileUpdate = true;
}
void TileMap::resetOffset() {
	_offset = glm::vec2(0);
}
void TileMap::changeDrawColor(Jauntlet::Color color) {
	_drawColor = color;
	_needsTileUpdate = true;
}

bool TileMap::isValidTilePos(glm::ivec2 position) {
	return !(position.y < 0 || position.y >= _level.size() || position.x >= _level[position.y].size() || position.x < 0);
}

void TileMap::updateTileMap() {
	_spriteBatch.begin();

	// Rendering all tiles into the sprite batch
	for (int y = 0; y < _level.size(); y++) {
		for (int x = 0; x < _level[y].size(); x++) {
			// Find and Process the tile
			unsigned int tile = _level[y][x];
			auto mapIterator = _tiles.find(tile);

			if (mapIterator == _tiles.end()) {
				continue;
			}
			
			// Create the location and size of the tile
			glm::vec4 destRect(x * _tileSize + _offset.x, -y * _tileSize + _offset.y, _tileSize, _tileSize);

			if (mapIterator->second.tileSet != nullptr) {
				// tile is a tileset, process which tile should be drawn
				unsigned int tileData = 0;

				if (testTileSetRules(mapIterator->second.tileSet, x + 1, y)) {
					tileData |= TileSet::TileSides::RIGHT;
				}

				if (testTileSetRules(mapIterator->second.tileSet, x - 1, y)) {
					tileData |= TileSet::TileSides::LEFT;
				}

				if (testTileSetRules(mapIterator->second.tileSet, x, y + 1)) {
					tileData |= TileSet::TileSides::BOTTOM;
					// check for corners
					if (tileData & TileSet::TileSides::RIGHT && !testTileSetRules(mapIterator->second.tileSet, x + 1, y + 1)) {
						tileData |= TileSet::TileSides::BOTTOM_RIGHT;
					}
					if (tileData & TileSet::TileSides::LEFT && !testTileSetRules(mapIterator->second.tileSet, x - 1, y + 1)) {
						tileData |= TileSet::TileSides::BOTTOM_LEFT;
					}
				}

				if (testTileSetRules(mapIterator->second.tileSet, x, y - 1)) {
					tileData |= TileSet::TileSides::TOP;
					// check for corners
					if (tileData & TileSet::TileSides::RIGHT && !testTileSetRules(mapIterator->second.tileSet, x + 1, y - 1)) {
						tileData |= TileSet::TileSides::TOP_RIGHT;
					}
					if (tileData & TileSet::TileSides::LEFT && !testTileSetRules(mapIterator->second.tileSet, x - 1, y - 1)) {
						tileData |= TileSet::TileSides::TOP_LEFT;
					}
				}

				TileSet::Tileinfo currentTile = mapIterator->second.tileSet->tileSetToTile(tileData);
				_spriteBatch.draw(destRect, { currentTile.UV.x, currentTile.UV.y, currentTile.UV.w, currentTile.UV.z }, _textureCache.getTexture(currentTile.texture).id, 0, _drawColor);
			}
			else {
				// normal tile, render it as usual.
				_spriteBatch.draw(destRect, _textureCache.getTexture(mapIterator->second.texture).id, 0, _drawColor);
			}
		}
	}
	_spriteBatch.end();
}

bool TileMap::testTileSetRules(TileSet* tile, int x, int y) {
	// make sure the position is within the level range
	if (y < 0 || y >= _level.size() || x >= _level[y].size() || x < 0) {
		return (tile->connectionRules & TileSet::ConnectionRules::EMPTY) ? true : false;
	}
	else {
		auto iterator = _tiles.find(_level[y][x]);
		
		if (iterator == _tiles.end()) { // must always check if the result is empty first
			return (tile->connectionRules & TileSet::ConnectionRules::EMPTY) ? true : false;
		}
		else if (iterator->second.tileSet != nullptr && iterator->second.tileSet->getID() == tile->getID()) { // check if the tile is the same tileset
			return true;
		}
		else if (iterator->second.tileSet != nullptr) { // check if the tile is a tileset
			return (tile->connectionRules & TileSet::ConnectionRules::TILESETS) ? true : false;
		}
		// The only remaining condition is if the tile is a regular tile.
		return (tile->connectionRules & TileSet::ConnectionRules::TILES) ? true : false;
	}
}

bool TileMap::shortestDist(std::pair<float, glm::vec2>& a, std::pair<float, glm::vec2>& b) {
	return a.first < b.first;
}
#pragma once

#include <functional>
#include <map>
#include <string>
#include <utility>
#include <vector>

#include "Collision/Collider2D.h"
#include "Rendering/GLTexture.h"
#include "Rendering/SpriteBatch.h"
#include "Rendering/TextureCache.h"
#include "TileSet.h"

namespace Jauntlet {

enum TileCollision { NONE = 0, SQUARE = 1};

// This is probably an unintuitive way to handle this, however we need to be able to efficentely link the chars from the level file to the texture or tileset based on what it is.
struct tile {
	std::string texture;
	TileSet* tileSet;
	std::function<void(int, int)> tileFunc;

	TileCollision tileCollision;

	tile(std::string Texture, TileCollision collisionType) {
		texture = Texture;
		tileSet = nullptr;
		tileFunc = nullptr;

		tileCollision = collisionType;
	}
	tile(TileSet* tileset, TileCollision collisionType) {
		tileSet = tileset;
		tileFunc = nullptr;

		tileCollision = collisionType;
	}
	tile(std::function<void(int, int)> func) {
		tileSet = nullptr;
		tileFunc = func;

		tileCollision = TileCollision::NONE;
	}
};

class TileMap
{
public:
	// loads in the tilemap
	TileMap(TextureCache& textureCache, int tileSize);
	// register a key to identify a tile
	void registerTile(char identifier, std::string filePath, TileCollision collisionType = TileCollision::NONE);
	// register a key to identify a tileSet. TileSetConnections should be from the "TileSetConnections" ENUM.
	void registerTileSet(char identifier, TileSet& tileSet, TileCollision collisionType = TileCollision::NONE);
	// register a key to execute a function at its location
	void registerFunction(char identifier, std::function<void(int, int)> customFunction);
	// loads tile map from a file of chars to place all the tiles in the world.
	// returns ID of tilemap, although the IDs are in load order so its unlikely to be needed.
	int loadTileMap(std::string filePath, float offsetX = 0, float offsetY = 0);
	// draw the tilemap on screen
	void draw();

	// returns adjacent tiles to position with collision. This is assuming the position is the top-left position on a square.
	std::vector<BoxCollider2D> collectCollidingTiles(glm::vec2 position, int levelIndex);
	// returns adjacent tiles to position with collision. This is assuming the position is the top-left position on a square.
	std::vector<BoxCollider2D> collectCollidingTiles(glm::vec2 position);
	// returns all tiles with collision within the bounds of the box collider
	std::vector<BoxCollider2D> collectCollidingTiles(BoxCollider2D collider, int levelIndex);
	// returns true if the tile position has a collision.
	bool tileHasCollision(glm::ivec2 tilePosition, int levelIndex);

	// Converts a position to be relative to the tileset: used mostly for checking tiles.
	glm::ivec2 WorldPosToTilePos(glm::vec2 position, int levelIndex);
	// Converts a position relative to the tileset to a position relative to the world.
	glm::vec2 TilePosToWorldPos(glm::ivec2 position, int levelIndex);
	// Rounds the world position to the nearest tile
	glm::vec2 RoundWorldPos(glm::vec2 position);
	
	// Checks if the specified position is a valid tile position. This prevents errors checking tiles in a non-existent location.
	bool isValidTilePos(int levelIndex, glm::ivec2 position);
private:
	// Updates what tiles are drawn to screen. Run this as little as possible.
	void updateTileMap(int levelIndex);
	
	// checks whether or not a tileset would connect to the specified tile position.
	bool testTileSetRules(TileSet tile, int x, int y, int levelIndex);

	// for sorting tiles via distance
	static bool shortestDist(std::pair<float, glm::vec2>& a, std::pair<float, glm::vec2>& b);

	std::vector<std::vector<std::string>> _levels;
	std::vector<SpriteBatch> _spriteBatches;

	int _tileSize;
	std::vector<glm::vec2> _offsets;
	std::map<char, tile> _tiles;

	TextureCache& _textureCache;
};
}
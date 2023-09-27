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

	TileCollision tileCollision;

	tile(std::string Texture, TileCollision collisionType) {
		texture = Texture;
		tileSet = nullptr;

		tileCollision = collisionType;
	}
	tile(TileSet* tileset, TileCollision collisionType) {
		tileSet = tileset;

		tileCollision = collisionType;
	}
};

class TileMap
{
public:
	// loads in the tilemap
	TileMap(TextureCache& textureCache, int tileSize);
	// Destructor
	~TileMap();

	// register a key to identify a tile
	void Register(std::string filePath, TileCollision collisionType = TileCollision::NONE);
	// register a key to identify a tileSet.
	void Register(TileSet& tileSet, TileCollision collisionType = TileCollision::NONE);
	
	// loads tile map from a file of chars to place all the tiles in the world.
	void loadTileMap(std::string filePath, float offsetX = 0, float offsetY = 0);
	
	// draw the tilemap on screen
	void draw();

	// returns adjacent tiles to position with collision. This is assuming the position is the top-left position on a square.
	std::vector<BoxCollider2D> collectCollidingTiles(glm::vec2 position);
	// returns all tiles with collision within the bounds of the box collider
	std::vector<BoxCollider2D> collectCollidingTiles(BoxCollider2D collider);
	
	// returns true if the tile position has a collision.
	bool tileHasCollision(glm::ivec2 tilePosition);
	// returns ID of tile position.
	unsigned int getTileID(glm::ivec2 tilePosition);

	// Converts a position to be relative to the tileset: used mostly for checking tiles.
	glm::ivec2 WorldPosToTilePos(glm::vec2 position);
	// Converts a position relative to the tileset to a position relative to the world.
	glm::vec2 TilePosToWorldPos(glm::ivec2 position);
	// Rounds the world position to the nearest tile
	glm::vec2 RoundWorldPos(glm::vec2 position);

	// Updates what a tile is based on ID. 
	// Running this repeatedly could be a performance bottleneck, especially on large tilemaps, as this recompiles the tile formations. Tilemaps only do this a max of once per frame.
	void UpdateTile(glm::ivec2 position, unsigned int newID);
	// Adds to the offset amount.
	// Running this repeatedly could be a performance bottleneck, especially on large tilemaps, as this recompiles the tile formations. Tilemaps only do this a max of once per frame.
	void AddOffset(glm::vec2 offset);

	// Checks if the specified position is a valid tile position. This prevents errors checking tiles in a non-existent location.
	bool isValidTilePos(glm::ivec2 position);
private:
	// Updates what tiles are drawn to screen. Run this as little as possible.
	void updateTileMap();
	
	// checks whether or not a tileset would connect to the specified tile position.
	bool testTileSetRules(TileSet tile, int x, int y);

	// for sorting tiles via distance
	static bool shortestDist(std::pair<float, glm::vec2>& a, std::pair<float, glm::vec2>& b);

	std::vector<std::vector<unsigned int>> _level;
	SpriteBatch _spriteBatch;

	int _tileSize;
	glm::vec2 _offset = glm::vec2(0);
	
	std::map<unsigned int, tile> _tiles;
	std::vector<TileSet*> _storedTileSets;
	int nextID = 1;
	bool _needsTileUpdate = false;

	TextureCache& _textureCache;
};
}
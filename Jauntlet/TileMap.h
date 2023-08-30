#pragma once
#include<string>
#include<vector>
#include<map>
#include<functional>

#include "Rendering/SpriteBatch.h"
#include "Rendering/GLTexture.h"
#include "Rendering/TextureCache.h"
#include "TileSet.h"
#include "Collision/Collider2D.h"
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
	// loads tile map from a file of chars to place all the tiles in the world
	void loadTileMap(std::string filePath, float offsetX = 0, float offsetY = 0);
	// draw the tilemap on screen
	void draw();

	// Grabs adjacent tiles to position and returns colliders from the tilemap
	std::vector<BoxCollider2D> collectCollidingTiles(glm::vec2 position);
private:
	// checks whether or not a tileset would connect to the specified tile position.
	bool testTileSetRules(TileSet tile, int x, int y);

	std::vector<std::string> _levelData;
	SpriteBatch _spriteBatch;

	int _tileSize;
	glm::vec2 _offset;

	std::map<char, tile> _tiles;

	TextureCache& _textureCache;
};
}
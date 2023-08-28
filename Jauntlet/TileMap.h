#pragma once
#include<string>
#include<vector>
#include<map>
#include<functional>

#include "Rendering/SpriteBatch.h"
#include "Rendering/GLTexture.h"
#include "Rendering/TextureCache.h"
#include "TileSet.h"
namespace Jauntlet {
	
// This is probably an unintuitive way to handle this, however we need to be able to efficentely link the chars from the level file to the texture or tileset based on what it is.
struct tile {
	std::string texture;
	TileSet* tileSet;

	std::function<void(int, int)> tileFunc;
	tile(std::string Texture) {
		texture = Texture;
		tileSet = nullptr;
		tileFunc = nullptr;
	}
	tile(TileSet* tileset) {
		tileSet = tileset;
		tileFunc = nullptr;
	}
	tile(std::function<void(int, int)> func) {
		tileSet = nullptr;
		tileFunc = func;
	}
};

class TileMap
{
public:
	// loads in the tilemap
	TileMap(TextureCache& textureCache, int tileSize);
	// register a key to identify a tile
	void registerTile(char identifier, std::string filePath);
	// register a key to identify a tileSet. TileSetConnections should be from the "TileSetConnections" ENUM.
	void registerTileSet(char identifier, TileSet& tileSet);
	// register a key to execute a function at its location
	void registerFunction(char identifier, std::function<void(int, int)> customFunction);
	// loads tile map from a file of chars to place all the tiles in the world
	void loadTileMap(std::string filePath, float offsetX = 0, float offsetY = 0);
	// draw the tilemap on screen
	void draw();
private:
	// checks whether or not a tileset would connect to the specified tile position.
	bool testTileSetRules(TileSet tile, int x, int y);

	std::vector<std::string> _levelData;
	SpriteBatch _spriteBatch;

	int _tileSize;

	std::map<char, tile> _tiles;

	TextureCache& _textureCache;
};
}
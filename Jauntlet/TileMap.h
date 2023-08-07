#pragma once
#include<string>
#include<vector>
#include<map>

#include "SpriteBatch.h"
#include "GLTexture.h"
#include "TextureCache.h"
#include "TileSet.h"
namespace Jauntlet {
// This is probably an unintuitive way to handle this but here are the reasons this solution was chosen
// 1. We use a map to connect chars with the information of tiles. We need to be able to put both tilesets and individual tiles in there.
// 2. We need to be able to tell the TileMap whether the tile is a TileSet or a individual tile. 
struct tile {
	std::string texture;
	bool isTileSet;
	TileSet* tileSet;

	tile(std::string Texture) {
		texture = Texture;
		isTileSet = false;
		tileSet = nullptr;
	}
	tile(TileSet* tileset) {
		texture = "";
		isTileSet = true;
		tileSet = tileset;
	}
};

class TileMap
{
public:
	// loads in the tilemap
	TileMap(TextureCache& textureCache, int tileSize);
	// register a key to identify a tile
	void registerTile(char identifier, std::string filePath);
	// register a key to identify a tileSet
	void registerTileSet(char identifier, TileSet& tileSet);
	// loads tile map from a file of chars to place all the tiles in the world
	void loadTileMap(std::string filePath, float offsetX = 0, float offsetY = 0);
	// draw the tilemap on screen
	void draw();
private:
	std::vector<std::string> _levelData;
	SpriteBatch _spriteBatch;

	int _tileSize;

	std::map<char, tile> _tiles;

	TextureCache& _textureCache;
};
}


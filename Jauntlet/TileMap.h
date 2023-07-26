#pragma once
#include<string>
#include<vector>
#include<map>

#include "SpriteBatch.h"
#include "GLTexture.h"

namespace Jauntlet {
	class TileMap
	{
	public:
		// loads in the tilemap
		TileMap(const std::string& fileName, int tileSize);

		// register a key to identify a tile
		void registerTile(char identifier, std::string filePath);
		// loads tile map from a file of chars to place all the tiles in the world
		void loadTileMap(std::string filePath);

		void draw();
	private:
		std::vector<std::string> _levelData;
		SpriteBatch _spriteBatch;

		int _tileSize;

		std::map<char, GLTexture> _tiles;
	};
}


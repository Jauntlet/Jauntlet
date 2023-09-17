#pragma once

#include <string>
#include <glm/glm.hpp>

namespace Jauntlet {
class TileSet
{
public:
	enum TileSides { RIGHT = 0x1, BOTTOM = 0x2, TOP = 0x4, LEFT = 0x8 };
	// The rules for Tilesets connecting to other tile types. NONE means it will only connect to itself.
	enum ConnectionRules { NONE = 0x0, TILESETS = 0x1, TILES = 0x2, EMPTY = 0x4 };

	struct Tileinfo {
		std::string texture;
		glm::vec4 UV;

		Tileinfo(std::string file) {
			texture = file;
			UV = glm::vec4(0, 0, 1, 1);
		}
		Tileinfo() {
			texture = "";
			UV = glm::vec4(0, 0, 1, 1);
		}
	};

	TileSet(std::string AllSidesCovered, std::string NoSidesCovered,
		std::string Right, std::string Left, std::string Bottom, std::string Top,
		std::string BottomRight, std::string BottomLeft, std::string BottomTop,
		std::string TopRight, std::string TopLeft, std::string RightLeft,
		std::string BottomTopLeft, std::string BottomTopRight, std::string BottomLeftRight, std::string TopRightLeft);
	// This constructor assumes a single image with a specific layout, please see examples provided.
	TileSet(std::string FilePath);
	
	void addConnectionRule(unsigned int rule);
	// Used by TileMaps to correctly calculate the tile to display
	TileSet::Tileinfo tileSetToTile(unsigned int layout);
	// Get the ID for the tileset, meant to differientiate different TileSets.
	const int getID() {
		return _id;
	}
	unsigned int connectionRules = 0;
private:

	int _id;
	
	TileSet::Tileinfo _mainTiles[16];
};
}
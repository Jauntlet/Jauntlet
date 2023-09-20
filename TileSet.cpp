#include "TileSet.h"

using namespace Jauntlet;

static int _lastestID = 0;

TileSet::TileSet(std::string AllSidesCovered, std::string NoSidesCovered,
	std::string Right, std::string Left, std::string Bottom, std::string Top,
	std::string BottomRight, std::string BottomLeft, std::string BottomTop,
	std::string TopRight, std::string TopLeft, std::string RightLeft,
	std::string BottomTopLeft, std::string BottomTopRight, std::string BottomLeftRight, std::string TopRightLeft) {
	_id = _lastestID;
	_lastestID++;
	_mainTiles[0] = TileSet::Tileinfo(NoSidesCovered);
	_mainTiles[1] = TileSet::Tileinfo(BottomTopLeft);
	_mainTiles[2] = TileSet::Tileinfo(TopRightLeft);
	_mainTiles[3] = TileSet::Tileinfo(TopLeft);
	_mainTiles[4] = TileSet::Tileinfo(BottomLeftRight);
	_mainTiles[5] = TileSet::Tileinfo(BottomLeft);
	_mainTiles[6] = TileSet::Tileinfo(RightLeft);
	_mainTiles[7] = TileSet::Tileinfo(Left);
	_mainTiles[8] = TileSet::Tileinfo(BottomTopRight);
	_mainTiles[9] = TileSet::Tileinfo(BottomTop);
	_mainTiles[10] = TileSet::Tileinfo(TopRight);
	_mainTiles[11] = TileSet::Tileinfo(Top);
	_mainTiles[12] = TileSet::Tileinfo(BottomRight);
	_mainTiles[13] = TileSet::Tileinfo(Bottom);
	_mainTiles[14] = TileSet::Tileinfo(Right);
	_mainTiles[15] = TileSet::Tileinfo(AllSidesCovered);
}

TileSet::TileSet(std::string FilePath) {
	for (int i = 0; i < 16; i++) {
		_mainTiles[i].texture = FilePath;
	}
	// UVs for a full image go from 0 -> 1. This is the size the UV needs to be for a single tile on the template image.
	// Do not that the Y axis goes from the bottom to top, not top to bottom.
	const float UVsize = 0.2f;

	_mainTiles[0 ].UV = glm::vec4(UVsize * 0, UVsize * 4, UVsize, UVsize);
	_mainTiles[1 ].UV = glm::vec4(UVsize * 0, UVsize * 3, UVsize, UVsize);
	_mainTiles[2 ].UV = glm::vec4(UVsize * 3, UVsize * 2, UVsize, UVsize);
	_mainTiles[3 ].UV = glm::vec4(UVsize * 0, UVsize * 2, UVsize, UVsize);
	_mainTiles[4 ].UV = glm::vec4(UVsize * 3, UVsize * 0, UVsize, UVsize);
	_mainTiles[5 ].UV = glm::vec4(UVsize * 0, UVsize * 0, UVsize, UVsize);
	_mainTiles[6 ].UV = glm::vec4(UVsize * 3, UVsize * 1, UVsize, UVsize);
	_mainTiles[7 ].UV = glm::vec4(UVsize * 0, UVsize * 1, UVsize, UVsize);
	_mainTiles[8 ].UV = glm::vec4(UVsize * 2, UVsize * 3, UVsize, UVsize);
	_mainTiles[9 ].UV = glm::vec4(UVsize * 1, UVsize * 3, UVsize, UVsize);
	_mainTiles[10].UV = glm::vec4(UVsize * 2, UVsize * 2, UVsize, UVsize);
	_mainTiles[11].UV = glm::vec4(UVsize * 1, UVsize * 2, UVsize, UVsize);
	_mainTiles[12].UV = glm::vec4(UVsize * 2, UVsize * 0, UVsize, UVsize);
	_mainTiles[13].UV = glm::vec4(UVsize * 1, UVsize * 0, UVsize, UVsize);
	_mainTiles[14].UV = glm::vec4(UVsize * 2, UVsize * 1, UVsize, UVsize);
	_mainTiles[15].UV = glm::vec4(UVsize * 1, UVsize * 1, UVsize, UVsize);
}
// Adds to what the Tileset can connect to. Use TileSet::ConnectionRules.
void TileSet::addConnectionRule(unsigned int rule) {
	connectionRules |= rule;
}

// used by tilemaps to find and display the correct tile based on surrounding tiles
TileSet::Tileinfo TileSet::tileSetToTile(unsigned int layout) {	
	// 1 = right side
	// 2 = bottom
	// 4 = top
	// 8 = left

	if (layout < 16) {
		return _mainTiles[layout];
	}
	
	return _mainTiles[0];
}
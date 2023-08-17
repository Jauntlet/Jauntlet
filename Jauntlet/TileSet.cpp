#include "TileSet.h"

using namespace Jauntlet;

static int _lastestID = 0;

TileSet::TileSet(std::string AllSidesCovered, std::string NoSidesCovered,
	std::string Right, std::string Left, std::string Bottom, std::string Top,
	std::string BottomRight, std::string BottomLeft, std::string BottomTop,
	std::string TopRight, std::string TopLeft, std::string RightLeft,
	std::string BottomTopLeft, std::string BottomTopRight, std::string BottomLeftRight, std::string TopRightLeft) : connectionRules(0) {
	_id = _lastestID;
	_lastestID++;
	_mainTiles[0] = NoSidesCovered;
	_mainTiles[1] = BottomTopLeft;
	_mainTiles[2] = TopRightLeft;
	_mainTiles[3] = TopLeft;
	_mainTiles[4] = BottomLeftRight;
	_mainTiles[5] = BottomLeft;
	_mainTiles[6] = RightLeft;
	_mainTiles[7] = Left;
	_mainTiles[8] = BottomTopRight;
	_mainTiles[9] = BottomTop;
	_mainTiles[10] = TopRight;
	_mainTiles[11] = Top;
	_mainTiles[12] = BottomRight;
	_mainTiles[13] = Bottom;
	_mainTiles[14] = Right;
	_mainTiles[15] = AllSidesCovered;
}
// Adds to what the Tileset can connect to. Use TileSet::ConnectionRules.
void TileSet::addConnectionRule(unsigned int rule) {
	connectionRules |= rule;
}

// used by tilemaps to find and display the correct tile based on surrounding tiles
std::string TileSet::tileSetToTile(unsigned int layout) {	
	// 1 = right side
	// 2 = bottom
	// 4 = top
	// 8 = left

	if (layout < 16) {
		return _mainTiles[layout];
	}
	
	return _mainTiles[0];
}
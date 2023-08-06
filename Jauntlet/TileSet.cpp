#include "TileSet.h"

using namespace Jauntlet;

// I am sorry for my crimes against humanity.
TileSet::TileSet(std::string AllSidesCovered, std::string NoSidesCovered,
	std::string Right, std::string Left, std::string Bottom, std::string Top,
	std::string BottomRight, std::string BottomLeft, std::string BottomTop,
	std::string TopRight, std::string TopLeft, std::string RightLeft,
	std::string BottomTopLeft, std::string BottomTopRight, std::string BottomLeftRight, std::string TopRightLeft) :
	_allSidesCovered(AllSidesCovered), _noSidesCovered(NoSidesCovered),
	_right(Right), _left(Left), _bottom(Bottom), _top(Top),
	_bottomRight(BottomRight), _bottomLeft(BottomLeft), _bottomTop(BottomTop),
	_topRight(TopRight), _topLeft(TopLeft), _rightLeft(RightLeft),
	_bottomTopLeft(BottomTopLeft), _bottomTopRight(BottomTopRight), _bottomLeftRight(BottomLeftRight), _topRightLeft(TopRightLeft) {
	
}

std::string TileSet::tileSetToTile(unsigned int layout) {
	
	// 1 = right side
	// 2 = bottom
	// 4 = top
	// 8 = left

	switch (layout) {
		case 1:
			return _bottomTopLeft;
		case 2:
			return _topRightLeft;
		case 3:
			return _topLeft;
		case 4:
			return _bottomLeftRight;
		case 5:
			return _bottomLeft;
		case 6:
			return _rightLeft;
		case 7:
			return _left;
		case 8:
			return _bottomTopRight;
		case 9:
			return _bottomTop;
		case 10:
			return _topRight;
		case 11:
			return _top;
		case 12: 
			return _bottomRight;
		case 13:
			return _bottom;
		case 14:
			return _right;
		case 15:
			return _allSidesCovered;
		default:
			return _noSidesCovered;
	}
}
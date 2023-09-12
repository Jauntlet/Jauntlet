#pragma once
#include <Jauntlet/TileMap.h>
#include <glm/glm.hpp>
#include <vector>

class Pathfinding
{
public:
	Pathfinding();
	// Finds the path between a starting and end location in world space, using a tilemap as reference.
	static std::vector<glm::vec2> findPath(Jauntlet::TileMap& map, glm::vec2 start, glm::vec2 destination, bool allowDiagonals = true);
private:
	static std::vector<std::pair<glm::vec2, glm::vec2>> _openList;
	static std::vector<std::pair<glm::vec2, glm::vec2>> _closedList;
};


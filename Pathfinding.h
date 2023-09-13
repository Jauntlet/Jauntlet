#pragma once
#include <Jauntlet/TileMap.h>
#include <glm/glm.hpp>
#include <vector>

struct cell {
	glm::vec2 position;
	float pathDistance;
	float estimatedDistance;
	cell* parent;

	cell(cell* cellParent, glm::vec2 pos) {
		parent = cellParent;
		position = pos;
		pathDistance = -1;
		estimatedDistance = -1;
	}
};

class Pathfinding
{
public:
	Pathfinding();
	// Finds the path between a starting and end location in world space, using a tilemap as reference.
	static std::vector<glm::vec2> findPath(Jauntlet::TileMap& map, glm::vec2 start, glm::vec2 destination, bool allowDiagonals = true);

private:

	static std::vector<cell> _openList;
	static std::vector<cell> _closedList;
};


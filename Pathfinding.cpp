#include "Pathfinding.h"
#include <Jauntlet/JMath.h>

std::vector<std::pair<glm::vec2, glm::vec2>> Pathfinding::_openList;
std::vector<std::pair<glm::vec2, glm::vec2>> Pathfinding::_closedList;

Pathfinding::Pathfinding() {
	// Empty
}
std::vector<glm::vec2> Pathfinding::findPath(Jauntlet::TileMap& map, glm::vec2 start, glm::vec2 destination, bool allowDiagonals /*= true*/) {	
	/* EXPLANATION OF LOGIC:
	* Firstly, you may notice that we are storing std::pair<glm::vec2, glm::vec2>, which is a bit weird and probably
	* difficult to discern. The first glm::vec2 is the position of the tile, which is expect
	* While the second glm::vec2 is stored for mathmatical operations. The first float is the total "score" of the tile,
	* with larger numbers being worse. The second one is the distance from the start, which is used for calculations for successor tiles
	* and is otherwise thrown away data.
	*/

	// translate world coords to tilemap coords.
	destination = map.WorldPosToTilePos(destination);
	start = map.WorldPosToTilePos(start);

	// reset lists
	_openList.clear();
	_closedList.clear();
	_openList.emplace_back(start, 0);

	int distFromStart = 0;
	while (!_openList.empty()) {
		int bestNodeID = 0;
		// Search through the list of nodes for the lowest movement cost
		for (int i = 1; i < _openList.size(); i++) {
			if (_openList[i].second.x < _openList[bestNodeID].second.x) {
				bestNodeID = i;
			}
		}
		// add best node to output list
		std::pair<glm::vec2, glm::vec2> bestNode = _openList[bestNodeID];

		// pop best node off the list
		_openList[bestNodeID] = _openList.back();
		_openList.pop_back();

		// Loop through all successors to the bestNode
		for (int y = -1; y < 2; y++) {
			for (int x = -1; x < 2; x++) {
				if (x == 0 && y == 0) { // skip (0,0)
					continue;
				}
				if (!allowDiagonals && x != 0 && y != 0) { // skip diagonals if they aren't allowed.
					continue;
				}

				std::pair<glm::vec2, glm::vec2> currentNode(bestNode.first + glm::vec2(x, y), 0);

				if (currentNode.first == destination) {
					// Goal was found, we are done.
				}

				// Position has collision, and therefore is not a valid position to check for navigation.
				if (map.tileHasCollision(currentNode.first)) {
					continue;
				}

				// Distance calculation: if diagonals are allowed we run a distance calc, otherwise the distance will always be 1
				currentNode.second.y = bestNode.second.y + (allowDiagonals ? std::abs(JMath::Distance(currentNode.first, bestNode.first)) : 1);

				// Calculate the distance from the node to the goal: this is essiential for A* pathfinding.
				if (!allowDiagonals) {
					// We use manhattan distance when we are not allowing diagonals: it is just the difference of X + the difference of Y
					currentNode.second.x = std::abs(currentNode.first.x - destination.x) + std::abs(currentNode.first.y - destination.y);
				}
				else {
					// When diagonals are allowed, we simply use a distance formula.
					currentNode.second.x = JMath::Distance(currentNode.first, destination);
				}

				// Final score:
				currentNode.second.x += currentNode.second.y;

				// Loop through the open list for tiles at the same position, with a lower score. If found, we skip this successor.
				for (int i = 0; i < _openList.size(); i++) {
					if (currentNode.first == _openList[i].first && currentNode.second.x > _openList[i].second.x) {
						continue;
					}
				}

				// Loop through the closed list for tiles at the same position, with a lower score. If found, we skip this successor.
				for (int i = 0; i < _closedList.size(); i++) {
					if (currentNode.first == _closedList[i].first && currentNode.second.x > _closedList[i].second.x) {
						continue;
					}
				}
				
				_openList.push_back(currentNode);
			}
		}

		_closedList.push_back(bestNode);
	}
	std::vector<glm::vec2> output;

	for (int i = 0; i < _closedList.size(); i++) {
		output.push_back(_closedList[i].first);
	}

	return output;
}
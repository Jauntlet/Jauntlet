#include "Pathfinding.h"
#include <Jauntlet/JMath.h>

#include <iostream> // remove when done debugging

std::vector<cell> Pathfinding::_openList;
std::vector<cell> Pathfinding::_closedList;

Pathfinding::Pathfinding() {
	// Empty
}
std::vector<glm::vec2> Pathfinding::findPath(Jauntlet::TileMap& map, glm::vec2 start, glm::vec2 destination, bool allowDiagonals /*= true*/) {	
	// translate world coords to tilemap coords.
	destination = map.WorldPosToTilePos(destination);
	start = map.WorldPosToTilePos(start);

	// reset lists
	_openList.clear();
	_closedList.clear();
	_openList.emplace_back(nullptr, start);

	bool foundDest = false;
	while (!_openList.empty() && !foundDest) {
		int bestNodeID = 0;
		// Search through the list of nodes for the lowest movement cost
		for (int i = 1; i < _openList.size(); i++) {
			if (_openList[i].estimatedDistance < _openList[bestNodeID].estimatedDistance) {
				bestNodeID = i;
			}
		}
		// cache best node
		_closedList.push_back(_openList[bestNodeID]);

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

				cell currentNode = cell(_closedList.back().position, _closedList.back().position);

				if (currentNode.position == destination) {
					foundDest = true;
					break;
				}

				// Position has collision, and therefore is not a valid position to check for navigation.
				if (map.tileHasCollision(currentNode.position)) {
					continue;
				}

				if (!allowDiagonals) {
					// Diagonals are not possible, so the added distance is always 1
					currentNode.pathDistance = _closedList.back().pathDistance + 1;

					// Calculate the distance from the node to the goal: this is essiential for A* pathfinding.
					// We use manhattan distance when we are not allowing diagonals: The difference of X + the difference of Y
					currentNode.estimatedDistance = std::abs(currentNode.position.x - destination.x) + std::abs(currentNode.position.y - destination.y);
				}
				else {
					currentNode.pathDistance = _closedList.back().pathDistance + std::abs(JMath::Distance(currentNode.position, _closedList.back().position));

					// Calculate the distance from the node to the goal: this is essiential for A* pathfinding.
					// When diagonals are allowed, we simply use a distance formula.
					currentNode.estimatedDistance = JMath::Distance(currentNode.position, destination);
				}

				// Final score:
				currentNode.estimatedDistance += currentNode.pathDistance;

				bool isValidNode = true;
				// Loop through the open list for tiles at the same position, with a lower score. If found, we skip this successor.
				for (int i = 0; i < _openList.size(); i++) {
					if (currentNode.position == _openList[i].position && currentNode.estimatedDistance >= _openList[i].estimatedDistance) {
						isValidNode = false;
						break;
					}
				}

				if (!isValidNode) continue;

				// Loop through the closed list for tiles at the same position, with a lower score. If found, we skip this successor.
				for (int i = 0; i < _closedList.size(); i++) {
					if (currentNode.position == _closedList[i].position && currentNode.estimatedDistance >= _closedList[i].estimatedDistance) {
						isValidNode = false;
						break;
					}
				}

				if (!isValidNode) continue;
				
				_openList.push_back(currentNode);
			}
			if (foundDest) break;
		}
	}
	std::vector<glm::vec2> output;

	cell Node = _closedList.back();
	output.push_back(Node.position);

	for (int i = _closedList.size() - 2; i > -1; i--) {
		if (_closedList[i].position == Node.prevPos) {
			Node = _closedList[i];
		
			output.push_back(map.TilePosToWorldPos(Node.position));
		}
	}

	return output;
}
#include "Navigation.h"

#include <cstdlib>

const int outcoveAmt = 4;
const int layerAmt = 3;

std::vector<std::vector<int>> Navigation::genNav() {
	std::vector<std::vector<int>> map = std::vector<std::vector<int>>();
	
	for (int y = 0; y < layerAmt; y++) {
		std::vector<int> layer = std::vector<int>();

		int temp = outcoveAmt + (rand() % 2 - 1); //outcoveAmt - 1, outcoveAmt, outcoveAmt + 1
		for (int x = 0; x < temp; x++) {
			layer.push_back(rand() % 3); //0, 1, 2
		}

		map.push_back(layer);
	}

	return map;
}
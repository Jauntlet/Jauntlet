#include "Navigation.h"

#include <chrono>
#include <random>

const int outcoveAmt = 4;
const int layerAmt = 3;
static int seed = std::chrono::system_clock::now().time_since_epoch().count(); //temp

Navigation::Navigation() {
	// Empty
}

std::vector<std::vector<int>> Navigation::genNav() {
	std::vector<std::vector<int>> map = std::vector<std::vector<int>>();
	std::mt19937 r = std::mt19937(seed);

	for (int y = 0; y < layerAmt; y++) {
		std::vector<int> layer = std::vector<int>();

		int temp = outcoveAmt + (r() % 2 - 1); //outcoveAmt - 1, outcoveAmt, outcoveAmt + 1
		for (int x = 0; x < temp; x++) {
			layer.push_back(r() % 3); //0, 1, 2
		}

		map.push_back(layer);
	}

	return map;
}

void Navigation::drawNav(std::vector<std::vector<int>>& navPoints, Jauntlet::SpriteFont& font, Jauntlet::SpriteBatch& spriteBatch) {
	if (!_navOpen) {
		return;
	}
	
	for (int y = 0; y < navPoints.size(); y++) {
		for (int x = 0; x < navPoints[y].size(); x++) {
			int point = navPoints[y][x];

			if (point == 0) { // white X
				font.draw(spriteBatch, "X", glm::vec2(x * 50, y * 50), glm::vec2(1), 0, Jauntlet::Color(255, 255, 255, 255));
				continue;
			}
			if (point == 1) { // blue X
				font.draw(spriteBatch, "X", glm::vec2(x * 50, y * 50), glm::vec2(1), 0, Jauntlet::Color(0, 0, 255, 255));
				continue;
			}
			if (point == 2) { // orange X
				font.draw(spriteBatch, "X", glm::vec2(x * 50, y * 50), glm::vec2(1), 0, Jauntlet::Color(255, 165, 0, 255));
				continue;
			}
		}
	}
}

void Navigation::toggleNav() {

}
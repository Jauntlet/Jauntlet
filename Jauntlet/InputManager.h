#pragma once
#include <unordered_map>
#include <glm/glm.hpp>

namespace Jauntlet {
class InputManager
{
public:
	InputManager();

	void pressKey(unsigned int keyID);
	void releaseKey(unsigned int keyID);
	
	void setMouseCoords(float x, float y);

	bool isKeyDown(unsigned int keyID);

	// getters
	glm::vec2 getMouseCoords() const { return _mouseCoords; }
private:
	std::unordered_map<unsigned int, bool> _keyMap;
	glm::vec2 _mouseCoords;
};
}
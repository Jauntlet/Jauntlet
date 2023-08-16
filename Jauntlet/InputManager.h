#pragma once
#include <unordered_map>
#include <glm/glm.hpp>
#include <SDL/SDL.h>

namespace Jauntlet {
class InputManager
{
public:
	InputManager();
	// processes all potential inputs. Returns true if the player presses QUIT
	bool processInput();
	// checks if a specific key is pressed down
	bool isKeyDown(unsigned int keyID);
	
	// Returns the last button pressed by the user.
	SDL_KeyCode lastButtonPressed();
	// sets the last button pressed to the ESCAPE key.
	void clearLastButtonPressed();

	// getters
	glm::vec2 getMouseCoords() const { return _mouseCoords; }
private:
	void pressKey(unsigned int keyID);
	void releaseKey(unsigned int keyID);	
	void setMouseCoords(float x, float y);
	
	std::unordered_map<unsigned int, bool> _keyMap;
	glm::vec2 _mouseCoords;

	SDL_KeyCode _lastInput;
};
}
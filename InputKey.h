#pragma once

#include <SDL/SDL.h>
#include <vector>

#include "InputManager.h"

namespace Jauntlet {
// Stores multiple keycodes together that have the same functionality
class InputKey
{
public:
	InputKey();
	InputKey(InputManager* inputManager);
	InputKey(InputManager* inputManager, SDL_KeyCode key);
	// initialize InputKey; for use when using empty constructor.
	void init(InputManager* inputManager);
	// add a key that will trigger the input
	void addKey(SDL_KeyCode key);
	// add a key that will trigger the input
	void addKey(SDL_KeyCode key, SDL_KeyCode keytwo);
	// add a key that will trigger the input
	void addKey(std::vector<SDL_KeyCode> keys);
	// remove a key from the input
	void removeKey(SDL_KeyCode key);
	// check if any of the keys are down
	bool isDown();
	// check if any of the keys are pressed
	bool isPressed();
private:
	 std::vector<SDL_KeyCode> _keys;
	 InputManager* _inputManager;
};
};


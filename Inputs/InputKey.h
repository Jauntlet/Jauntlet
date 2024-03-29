/* Purpose: Used in parallel with InputManager to be able to combine different key inputs into the same event
 * Main Contributer(s): Xander Mooney
 */
#pragma once

#include <vector>
#include "InputManager.h"

// Stores multiple keycodes together that have the same functionality
class InputKey
{
public:
	InputKey(InputManager* inputManager);
	InputKey(InputManager* inputManager, SDL_KeyCode key);
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
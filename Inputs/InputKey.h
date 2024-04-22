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
	void addKey(uint32_t key);
	// add a key that will trigger the input
	void addKey(uint32_t key, uint32_t keytwo);
	// add a key that will trigger the input
	void addKey(std::vector<uint32_t> keys);
	// remove a key from the input
	void removeKey(uint32_t key);
	// check if any of the keys are down
	bool isDown();
	// check if any of the keys are pressed
	bool isPressed();
private:
	 std::vector<uint32_t> _keys;
	 InputManager* _inputManager;
};
#pragma once
#include<SDL/SDL.h>
#include<vector>

#include"InputManager.h"
namespace Jauntlet {
class InputKey
{
public:
	InputKey(InputManager* inputManager);
	InputKey(InputManager* inputManager, SDL_KeyCode key);
	// add a key that will trigger the input
	void addKey(SDL_KeyCode key);
	// remove a key from the input
	void removeKey(SDL_KeyCode key);
	// check if any of the keys are pressed
	bool isPressed();
private:
	 std::vector<SDL_KeyCode> _keys;
	 InputManager* _inputManager;
}
};


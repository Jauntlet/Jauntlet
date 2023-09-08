#pragma once

#include <SDL/SDL.h>
#include <glm/glm.hpp>
#include <unordered_map>

namespace Jauntlet {
// Stores and manages the state of keys, aswell as other SDL events
class InputManager
{
public:
	InputManager();
	// Destructor for InputManager
	~InputManager();

	// processes all potential inputs. Returns true if the player presses QUIT
	void processInput();
	
	// checks if a specific key is pressed down
	bool isKeyDown(unsigned int keyID);
	// checks if a specific key was just pressed
	bool isKeyPressed(unsigned int keyID);

	// Returns the last button pressed by the user.
	SDL_KeyCode lastButtonPressed();
	// sets the last button pressed to the ESCAPE key.
	void clearLastButtonPressed();
	
	// returns true if the window has been resized since the last time the method was called.
	bool windowResized();
	// returns true if the user has pressed a button to quit the game.
	bool quitGameCalled();

	// getters
	glm::vec2 getMouseCoords();

	Sint32 deltaScroll = 0;

	// Get the axis of the left stick on a controller
	glm::vec2 getLeftStickAxis(int controllerID);
	// Get the axis of the right stick on a controller
	glm::vec2 getRightStickAxis(int controllerID);
private:
	void pressKey(unsigned int keyID);
	void releaseKey(unsigned int keyID);
	
	std::unordered_map<unsigned int, bool> _keyMap;
	std::unordered_map<unsigned int, bool> _previousKeyMap;
	glm::vec2 _mouseCoords = glm::vec2(0);

	SDL_KeyCode _lastInput = SDLK_ESCAPE;

	bool _windowResized = false;
	bool _quitGameCalled = false;

	// Controller Handling
	std::vector<SDL_Joystick*> _controller;
	std::vector<glm::vec2> _leftStickAxis;
	std::vector<glm::vec2> _rightStickAxis;
};
}
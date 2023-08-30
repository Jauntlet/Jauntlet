#pragma once
#include <unordered_map>
#include <glm/glm.hpp>
#include <SDL/SDL.h>

namespace Jauntlet {
// Stores and manages the state of keys, aswell as other SDL events
class InputManager
{
public:
	InputManager();
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
	glm::vec2 getMouseCoords() const { return _mouseCoords; }
private:
	void pressKey(unsigned int keyID);
	void releaseKey(unsigned int keyID);	
	void setMouseCoords(float x, float y);
	
	std::unordered_map<unsigned int, bool> _keyMap;
	std::unordered_map<unsigned int, bool> _previousKeyMap;
	glm::vec2 _mouseCoords;

	SDL_KeyCode _lastInput;

	bool _windowResized;
	bool _quitGameCalled;
};
}
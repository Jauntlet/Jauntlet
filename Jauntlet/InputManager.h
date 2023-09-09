#pragma once

#include <SDL/SDL.h>
#include <glm/glm.hpp>
#include <unordered_map>
#include <vector>

namespace Jauntlet {
enum class Axis { LeftStick, RightStick, Triggers };
	
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

	// Collect an Axis from a controller, this value is NOT normalized
	glm::vec2 getAxis(Axis type, int controllerID);
	// Collect the average of all Axis's from all controllers, this value is NOT normalized
	glm::vec2 getAxis(Axis type);

	Sint32 deltaScroll = 0;
private:
	std::unordered_map<unsigned int, bool> _keyMap;
	std::unordered_map<unsigned int, bool> _previousKeyMap;
	glm::vec2 _mouseCoords = glm::vec2(0);

	SDL_KeyCode _lastInput = SDLK_ESCAPE;

	bool _windowResized = false;
	bool _quitGameCalled = false;

	// Controller Handling
	struct Controller {
		SDL_Joystick* joystick;
		glm::vec2 leftStick;
		glm::vec2 rightStick;
		glm::vec2 triggers;

		Controller() {
			joystick = nullptr;
			leftStick = glm::vec2(0);
			rightStick = glm::vec2(0);
			triggers = glm::vec2(0);
		}
	};

	std::vector<Controller> _controllers;
};
}
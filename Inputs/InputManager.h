/* Purpose: Handles all possible user inputs for later reading during runtime.
 * Main Contributer(s): Xander Mooney
 */
#pragma once

#include <SDL2/SDL_events.h>
#include <glm/vec2.hpp>
#include <glm/fwd.hpp>
#include <unordered_map>
#include <vector>

enum class Axis { LeftStick, RightStick, Triggers, dPad };
	
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
	// checks if a specific key was just unpressed
	bool onKeyUp(unsigned int keyID);
	// checks if a specific key is unpressed
	bool isKeyUp(unsigned int keyID);

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
	glm::vec2 getControllerAxis(Axis type, int controllerID);
	// Collect the average of all Axis's from all controllers, this value is NOT normalized
	glm::vec2 getControllerAxis(Axis type);

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
		glm::vec2 dPad;

		Controller() {
			joystick = nullptr;
			leftStick = glm::vec2(0);
			rightStick = glm::vec2(0);
			triggers = glm::vec2(0);
			dPad = glm::vec2(0);
		}
	};

	std::vector<Controller> _controllers;
};
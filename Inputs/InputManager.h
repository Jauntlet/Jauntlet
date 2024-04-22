/* Purpose: Handles all possible user inputs for later reading during runtime.
 * Main Contributer(s): Xander Mooney
 */
#pragma once

#include <SDL2/SDL_events.h>
#include <SDL2/SDL_gamecontroller.h>
#include <glm/vec2.hpp>
#include <glm/fwd.hpp>
#include <unordered_map>
#include <vector>

enum class Axis { LeftStick, RightStick, Triggers, dPad };

// Controller Handling
struct Controller {
	SDL_GameController* controller;
	SDL_Joystick* joystick;
	SDL_GameControllerType type;
	glm::vec2 leftStick;
	glm::vec2 rightStick;
	glm::vec2 triggers;
	glm::vec2 dPad;

	Controller(SDL_GameController* Controller) {
		controller = Controller;
		joystick = SDL_GameControllerGetJoystick(Controller);
		type = SDL_GameControllerGetType(Controller);
		leftStick = glm::vec2();
		rightStick = glm::vec2();
		triggers = glm::vec2();
		dPad = glm::vec2();
	}
};

#define CONTROLLER_FACE_NORTH 200
#define CONTROLLER_FACE_EAST 201
#define CONTROLLER_FACE_SOUTH 202
#define CONTROLLER_FACE_WEST 203
#define CONTROLLER_BACK 204
#define CONTROLLER_GUIDE 205
#define CONTROLLER_START 206
#define CONTROLLER_LEFTSTICK 207
#define CONTROLLER_RIGHTSTICK 208
#define CONTROLLER_LEFTSHOULDER 209
#define CONTROLLER_RIGHTSHOULDER 210
#define CONTROLLER_DPAD_UP 211
#define CONTROLLER_DPAD_DOWN 212
#define CONTROLLER_DPAD_LEFT 213
#define CONTROLLER_DPAD_RIGHT 214
// Misc button contains the following cases:
// Xbox Series X share button
// Nintendo Switch Pro capture button
// Amazon Luna / PS5 Microphone button
#define CONTROLLER_MISC 215
#define CONTROLLER_PADDLE1 216
#define CONTROLLER_PADDLE2 217
#define CONTROLLER_PADDLE3 218
#define CONTROLLER_PADDLE4 219
#define CONTROLLER_TOUCHPAD 220
#define CONTROLLER_MAX 221

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
	uint32_t lastButtonPressed();
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
	// Returns the controller at the specified index.
	Controller* getController(int controllerID);
	// Returns the amount of connected controllers
	uint32_t getControllerCount();

	Sint32 deltaScroll = 0;
private:
	std::unordered_map<unsigned int, bool> _keyMap;
	std::unordered_map<unsigned int, bool> _previousKeyMap;
	glm::vec2 _mouseCoords = glm::vec2(0);

	unsigned int _lastInput = SDLK_ESCAPE;

	bool _windowResized = false;
	bool _quitGameCalled = false;

	std::vector<Controller> _controllers;
};
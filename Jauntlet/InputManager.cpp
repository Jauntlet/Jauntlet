#include <SDL/SDL.h>
#include <cstdio>

#include "InputManager.h"

using namespace Jauntlet;

InputManager::InputManager() {
	// Empty
}

InputManager::~InputManager() {
	for (int i = 0; i < _controllers.size(); i++) {
		SDL_JoystickClose(_controllers[i].joystick);
		_controllers[i].joystick = nullptr;
	}
}

void InputManager::processInput() {
	// Add controller if detected.
	while (SDL_NumJoysticks() > _controllers.size()) {
		_controllers.emplace_back();
		_controllers[_controllers.size() - 1].joystick = SDL_JoystickOpen(_controllers.size() - 1);
		printf("controller added\n");
	}

	// updates previous key map via foreach loop
	for (auto& it : _keyMap) {
		_previousKeyMap[it.first] = it.second;
	}

	// I wanted to declare this only within the case statement where we are dealing with controllerAxis's,
	// but doing so creates a "compiler error" where the variable is non-accessible to other case statements.
	// I disagree fully with this. This is stupid and dumb. -xm
	float controllerAxis = 0;

	SDL_Event _event;
	while (SDL_PollEvent(&_event))
	{
		switch (_event.type) {
			case SDL_QUIT:
				_quitGameCalled = true;
				break;
			case SDL_KEYDOWN:
				_keyMap[_event.key.keysym.sym] = true;
				_lastInput = (SDL_KeyCode)_event.key.keysym.sym;
				break;
			case SDL_KEYUP:
				_keyMap[_event.key.keysym.sym] = false;
				break;
			case SDL_JOYAXISMOTION:
				controllerAxis = _event.jaxis.value; // convert from int to float

				if (controllerAxis < 0) { // add one to negative values so that both positive and negative values are between the same range.
					controllerAxis += 1;
				}
				controllerAxis /= 32767; // divide by integer limit to get a number between -1 and 1.

				switch (_event.jaxis.axis) {
					case 0: // X Axis on main stick
						_controllers[_event.jaxis.which].leftStick.x = controllerAxis;
						break;
					case 1: // Y Axis on main stick
						_controllers[_event.jaxis.which].leftStick.y = controllerAxis;
						break;
					case 2: // X Axis on right stick
						_controllers[_event.jaxis.which].rightStick.x = controllerAxis;
						break;
					case 3: // Y Axis on right stick
						_controllers[_event.jaxis.which].rightStick.y = controllerAxis;
						break;
					case 4: // Left trigger
						_controllers[_event.jaxis.which].triggers.x = controllerAxis;
						break;
					case 5: // Right trigger
						_controllers[_event.jaxis.which].triggers.y = controllerAxis;
						break;
					default:
						printf("WARNING: undefined stick axis detected %i\n", _event.jaxis.axis);
						continue;
				}
				break;
			case SDL_MOUSEBUTTONDOWN:
				_keyMap[_event.button.button] = true;
				break;
			case SDL_MOUSEBUTTONUP:
				_keyMap[_event.button.button] = false;
				break;
			case SDL_MOUSEMOTION:
				_mouseCoords.x = _event.motion.x;
				_mouseCoords.y = _event.motion.y;
				break;
			case SDL_WINDOWEVENT:
				if (_event.window.event == SDL_WINDOWEVENT_SIZE_CHANGED) {
					_windowResized = true;
				}
				break;
			case SDL_MOUSEWHEEL:
				deltaScroll += _event.wheel.y;
				break;
		}
	}
}

bool InputManager::isKeyDown(unsigned int keyID) {
	return _keyMap[keyID];
}

bool InputManager::isKeyPressed(unsigned int keyID) {
	return _keyMap[keyID] ? !_previousKeyMap[keyID] : false;
}

SDL_KeyCode InputManager::lastButtonPressed() {
	return _lastInput;
}
void InputManager::clearLastButtonPressed() {
	_lastInput = SDLK_ESCAPE;
};

bool InputManager::windowResized() {
	if (_windowResized) {
		_windowResized = false;
		return true;
	}
	return false;
}

bool InputManager::quitGameCalled() {
	return _quitGameCalled;
}

glm::vec2 InputManager::getMouseCoords() {
	return _mouseCoords;
}

glm::vec2 InputManager::getAxis(Axis type, int controllerID) {
	if (controllerID >= _controllers.size()) {
		printf("Warning: Tried to get Axis on non-existant Controller");
		return glm::vec2(0);
	}
	//printf("Controller %i has left stick of %f %f\n", controllerID, _controllers[controllerID].leftStick.x, _controllers[controllerID].leftStick.y);
	switch (type) {
		case Axis::LeftStick:
			return _controllers[controllerID].leftStick;
		case Axis::RightStick:
			return _controllers[controllerID].rightStick;
		case Axis::Triggers:
			return _controllers[controllerID].triggers;
	}
}
glm::vec2 InputManager::getAxis(Axis type) {

}
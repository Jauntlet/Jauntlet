#include <SDL/SDL.h>

#include "InputManager.h"

using namespace Jauntlet;

InputManager::InputManager() {
	// Empty
}

InputManager::~InputManager() {
	for (int i = 0; i < _controller.size(); i++) {
		SDL_JoystickClose(_controller[i]);
		_controller[i] = nullptr;
	}
}

void InputManager::processInput() {
	// Add controllers if detected.
	if (SDL_NumJoysticks() > _controller.size()) {
		_controller.push_back(SDL_JoystickOpen(_controller.size()));
	}

	// updates previous key map via foreach loop
	for (auto& it : _keyMap) {
		_previousKeyMap[it.first] = it.second;
	}

	SDL_Event _event;
	while (SDL_PollEvent(&_event))
	{
		switch (_event.type)
		{
		case SDL_QUIT:
			_quitGameCalled = true;
			break;
		case SDL_KEYDOWN:
			pressKey(_event.key.keysym.sym);
			_lastInput = (SDL_KeyCode)_event.key.keysym.sym;
			break;
		case SDL_KEYUP:
			releaseKey(_event.key.keysym.sym);
			break;
		case SDL_JOYAXISMOTION:
			while (_leftStickAxis.size() <= _event.jaxis.which) {
				_leftStickAxis.push_back(glm::vec2(0));
				_rightStickAxis.push_back(glm::vec2(0));
			}
			switch (_event.jaxis.axis) {
				case 0: // X Axis on main stick
					_leftStickAxis[_event.jaxis.which].x = _event.jaxis.value;
					break;
				case 1: // Y Axis on main stick
					_leftStickAxis[_event.jaxis.which].y = _event.jaxis.value;
					break;
				case 2: // X Axis on right stick
					_rightStickAxis[_event.jaxis.which].x = _event.jaxis.value;
					break;
				case 3: // Y Axis on right stick
					_rightStickAxis[_event.jaxis.which].y = _event.jaxis.value;
					break;
				case 4: // Left trigger
					break;
				case 5: // Right trigger
					break;
				default:
					printf("WARNING: undefined stick axis detected %i\n", _event.jaxis.axis);
					return;
					break;
			}
			break;
		case SDL_MOUSEBUTTONDOWN:
			pressKey(_event.button.button);
			break;
		case SDL_MOUSEBUTTONUP:
			releaseKey(_event.button.button);
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

void InputManager::pressKey(unsigned int keyID) {
	_keyMap[keyID] = true;
}

void InputManager::releaseKey(unsigned int keyID) {
	_keyMap[keyID] = false;
}

glm::vec2 InputManager::getMouseCoords() {
	return _mouseCoords;
}

glm::vec2 InputManager::getLeftStickAxis(int controllerID) {
	if (controllerID >= _leftStickAxis.size()) {
		return glm::vec2(0);
	}
	return glm::normalize(_leftStickAxis[controllerID]);
}

glm::vec2 InputManager::getRightStickAxis(int controllerID) {
	if (controllerID >= _rightStickAxis.size()) {
		return glm::vec2(0);
	}
	return glm::normalize(_rightStickAxis[controllerID]);
}
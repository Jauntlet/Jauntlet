#include <SDL/SDL.h>

#include "InputManager.h"

using namespace Jauntlet;

InputManager::InputManager() {
	// Empty
}

void InputManager::processInput() {
	SDL_Event _event;

	// updates previous key map via foreach loop
	for (auto& it : _keyMap) {
		_previousKeyMap[it.first] = it.second;
	}

	while (SDL_PollEvent(&_event))
	{
		switch (_event.type)
		{
		case SDL_QUIT:
			_quitGameCalled = true;
			break;
		case SDL_KEYDOWN:
			pressKey(_event.key.keysym.sym);
			break;
		case SDL_KEYUP:
			releaseKey(_event.key.keysym.sym);
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
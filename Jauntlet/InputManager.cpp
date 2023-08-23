#include "InputManager.h"
#include<SDL/SDL.h>
using namespace Jauntlet;

InputManager::InputManager(): _mouseCoords(0), _lastInput(SDLK_ESCAPE) {

}

bool InputManager::processInput() {
	SDL_Event evnt;

	while (SDL_PollEvent(&evnt))
	{
		switch (evnt.type)
		{
		case SDL_QUIT:
			return true;
		case SDL_KEYDOWN:
			pressKey(evnt.key.keysym.sym);
			break;
		case SDL_KEYUP:
			releaseKey(evnt.key.keysym.sym);
			break;
		case SDL_MOUSEBUTTONDOWN:
			pressKey(evnt.button.button);
			break;
		case SDL_MOUSEBUTTONUP:
			releaseKey(evnt.button.button);
			break;
		case SDL_MOUSEMOTION:
			setMouseCoords(evnt.motion.x, evnt.motion.y);
			break;
		case SDL_WINDOWEVENT:
			if (evnt.window.event == SDL_WINDOWEVENT_RESIZED) {
				// window has been resized
			}
		}
	}

	return false;
}

bool InputManager::isKeyDown(unsigned int keyID) {
	return _keyMap[keyID];
}

SDL_KeyCode InputManager::lastButtonPressed() {
	return _lastInput;
}
void InputManager::clearLastButtonPressed() {
	_lastInput = SDLK_ESCAPE;
};

void InputManager::pressKey(unsigned int keyID) {
	_keyMap[keyID] = true;
}

void InputManager::releaseKey(unsigned int keyID) {
	_keyMap[keyID] = false;
}

void InputManager::setMouseCoords(float x, float y) {
	_mouseCoords.x = x;
	_mouseCoords.y = y;
}

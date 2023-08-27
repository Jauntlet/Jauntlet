#include "InputKey.h"
using namespace Jauntlet;

InputKey::InputKey(InputManager* inputManager) : _inputManager(inputManager) {
	
}
InputKey::InputKey(InputManager* inputManager, SDL_KeyCode key) : _inputManager(inputManager) {
	_keys.push_back(key);
}

void InputKey::addKey(SDL_KeyCode key) {
	_keys.push_back(key);
}
void InputKey::removeKey(SDL_KeyCode key) {
	for (int i = 0; i < _keys.size(); i++) {
		if (_keys[i] == key) {
			_keys[i] = _keys.back();
			_keys.pop_back();
			return;
		}
	}
}

bool InputKey::isDown() {
	for (int i = 0; i < _keys.size(); i++) {
		if (_inputManager->isKeyDown(_keys[i])) {
			return true;
		}
	}
	return false;
}
bool InputKey::isPressed() {
	for (int i = 0; i < _keys.size(); i++) {
		if (_inputManager->isKeyPressed(_keys[i])) {
			return true;
		}
	}
	return false;
}
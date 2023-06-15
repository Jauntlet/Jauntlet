#include "InputManager.h"

using namespace Jauntlet;

InputManager::InputManager() {

}

void InputManager::pressKey(unsigned int keyID) {
	_keyMap[keyID] = true;
}

void InputManager::releaseKey(unsigned int keyID) {
	_keyMap[keyID] = false;
}

bool InputManager::isKeyDown(unsigned int keyID) {
	return _keyMap[keyID];
}
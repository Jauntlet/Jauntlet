#include "UIManager.h"
#include "UIElement.h"
#include <iostream>

using namespace Jauntlet;

UIManager::UIManager() {
	// cock
}

UIManager::UIManager(Camera2D camera, SpriteBatch spriteBatch) {
	_camera = camera;
	_spriteBatch = spriteBatch;
}

void UIManager::fixResolution() {
	for (int i = 0; i < _uiElements.size(); i++) {
		_uiElements[i].fixResolution();
	}
}

void UIManager::addElement(UIElement uiElement) {
	_uiElements.push_back(uiElement);
}

void UIManager::removeElement(UIElement uiElement) {
	/*for (int i = 0; i < _uiElements.size(); i++) {
		if (_uiElements[i] == uiElement) {
			_uiElements[i] = _uiElements.back();
			_uiElements.pop_back();
			return;
		}
	}*/
	fatalError("error: unable to remove UIElement, dont try to do this!");
}

void UIManager::setScale(float scale) {
	_scale = scale;
}

void UIManager::update() {
	std::cout << _uiElements.size() << std::endl;
	for (int i = 0; i < _uiElements.size(); i++) {
		_uiElements[i].update(_camera, _spriteBatch, _scale);
	}
}
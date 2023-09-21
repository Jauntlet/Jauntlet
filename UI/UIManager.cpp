#include "UIManager.h"
#include "UIElement.h"
#include <iostream>

using namespace Jauntlet;

UIManager::UIManager() {
	// cock
}

UIManager::UIManager(Camera2D* camera, SpriteBatch* spriteBatch) {
	_camera = camera;
	_spriteBatch = spriteBatch;
	_scale = new glm::vec2(1);
}

void UIManager::addElement(UIElement* uiElement) {
	_uiElements.push_back(uiElement);
	uiElement->resolvePosition(_camera);
}

void UIManager::draw() {
	for (int i = 0; i < _uiElements.size(); i++) {
		_uiElements[i]->draw(_camera, _spriteBatch, _scale);
	}
}

void UIManager::setScale(float scale) {
	_scale = new glm::vec2(scale);
}

void UIManager::setScale(glm::vec2 scale) {
	_scale = &scale;
}

void UIManager::resolvePositions() {
	for (int i = 0; i < _uiElements.size(); i++) {
		_uiElements[i]->resolvePosition(_camera);
	}
}
#include "UIManager.h"
#include "UIElement.h"

using namespace Jauntlet;

UIManager::UIManager(Camera2D* camera, SpriteBatch& spriteBatch) {
	_camera = camera;
	_spriteBatch = &spriteBatch; // please someone tell me why i have to do this -jk
}

void UIManager::fixResolution() {
	for (int i = 0; i < _uiElements.size(); i++) {
		_uiElements[i]->fixResolution();
	}
}

void UIManager::addElement(UIElement* uiElement) {
	if (uiElement != nullptr) {
		_uiElements.push_back(uiElement);
	} else {
		fatalError("error: gave UIManager nonexistant camera or nullptr");
	}
}

void UIManager::removeElement(UIElement* uiElement) {
	for (int i = 0; i < _uiElements.size(); i++) {
		if (_uiElements[i] == uiElement) {
			_uiElements[i] = _uiElements.back();
			_uiElements.pop_back();
			return;
		}
	}
	fatalError("error: unable to remove UIElement, dont try to do this!");
}

void UIManager::setScale(float scale) {
	// considering using pointers of floats because maybe its faster idk i cant test that shit with a full time job -jk
	_scale = scale;
}

void UIManager::update() {
	for (int i = 0; i < _uiElements.size(); i++) {
		_uiElements[i]->update(_camera, *_spriteBatch, _scale); // does this dereference a point of a pointer? fuck you. -jk
	}
}
#include "UIManager.h"
#include "UIElement.h"

namespace Jauntlet {
	UIManager::UIManager(Camera2D*  camera) {
		if (camera != nullptr) {
			_camera = camera;
		} else {
			fatalError("error: gave UIManager nonexistant camera or nullptr");
		}
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

	void UIManager::update() {
		for (int i = 0; i < _uiElements.size(); i++) {
			_uiElements[i]->update();
		}
	}
	
}


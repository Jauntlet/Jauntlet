#include "UIManager.h"
#include "SDL/SDL_video.h"
#include "glm/fwd.hpp"

using namespace Jauntlet;

UIManager::UIManager() {
	// empty
}

UIManager::UIManager(Camera2D* camera) {
	_camera = camera;
	_spriteBatch.init();
}

void UIManager::addElement(UIElement* uiElement) {
	_uiElements.push_back(uiElement);
	uiElement->resolvePosition(_camera, _calculatedOriginPinPositionsInScreenspace, _scale);
}

void UIManager::draw() {
	_spriteBatch.begin();
	for (int i = 0; i < _uiElements.size(); ++i) {
		if (_uiElements[i]->visible) {
			_uiElements[i]->draw(_camera, &_spriteBatch, _scale);
		}
	}
	_spriteBatch.endAndRender();
}

void UIManager::setScale(float scale) {
	_scale = scale;
}

void UIManager::resolvePositions() {
	// we've changed our resolution, recalculate
	_recalculateOriginPinPositions();
	for (int i = 0; i < _uiElements.size(); ++i) {
		if (_uiElements[i]->visible) {
			_uiElements[i]->resolvePosition(_camera, _calculatedOriginPinPositionsInScreenspacePtr, _scale);
		}
	}
}
/*

breif reminder for those watching at home how screen space works.
0.0 on both axis is the top left, 1.0 on both axis is the top right.
obviously this only applies to a resolution of 1x1 but can be expaned on,
simply just multiply these values by the cameras width and height.

(0.0 , 0.0)       (0.5 , 0.0)       (1.0 , 0.0)









(0.0 , 0.5)       (0.5 , 0.5)       (1.0 , 0.5)









(0.0 , 1.0)       (0.5 , 1.0)       (1.0 , 1.0)

*/


void UIManager::_recalculateOriginPinPositions() {
	glm::vec2 _cameraSize = _camera->getCameraSize();
	_cameraSize.y *= -1; // corrects button positions
	for (int i = 0; i < 9; ++i) {
		_calculatedOriginPinPositionsInScreenspace[i] = _cameraSize * ORIGIN_PIN_POSITIONS[i];
	}
	// this pointer only works once, so every time we recalculate we pointerize it again
	_calculatedOriginPinPositionsInScreenspacePtr = _calculatedOriginPinPositionsInScreenspace;
}
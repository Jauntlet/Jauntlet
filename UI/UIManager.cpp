#include "UIManager.h"
#include "glm/fwd.hpp"

using namespace Jauntlet;

UIManager::UIManager() {
	// empty
}

UIManager::UIManager(Camera2D* camera) {
	_camera = camera;
}

void UIManager::addElement(UIElement* uiElement, GLSLProgram* program) {
	_uiElements.push_back(uiElement);
	_programs.push_back(program);
}

void UIManager::optimize() {

	for(int i = 0; i < _uiElements.size(); ++i) {
		GLSLProgram* program = _programs[i];
		UIBatch temporaryBatch(program, _uiElements[i]);

		int addedElements = 0;

		for (int i2 = i + 1; i2 < _uiElements.size(); ++i2) {
			if (_programs[i2] == program) {
				temporaryBatch.addElement(_uiElements[i2]);
				++addedElements;
			}
		}

		_uiBatches.push_back(temporaryBatch);

		i += addedElements;
	}

}

void UIManager::draw() {

	for (int i = 0; i < _uiBatches.size(); ++i) {
		_uiBatches[i].draw(_camera, _scale);
	}
}

void UIManager::setScale(float scale) {
	_scale = &scale;
}

void UIManager::resolvePositions() {
	// we've changed our resolution, recalculate
	_recalculateOriginPinPositions();
	for (int i = 0; i < _uiElements.size(); ++i) {
		if (_uiElements[i]->visible) {
			_uiElements[i]->resolvePosition(_camera, _calculatedOriginPinPositionsInScreenspacePtr, *_scale);
		}
	}
}
/*

brief reminder for those watching at home how screen space works.
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
}
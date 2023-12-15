#include "UIManager.h"
#include "glm/fwd.hpp"

using namespace Jauntlet;

UIManager::UIManager(Camera2D* camera) {
	_camera = camera;
}

void UIManager::addElement(UIElement* uiElement, GLSLProgram* program) {
	_uiElements.push_back(uiElement);
	_programs.push_back(program);
}

void UIManager::removeElement(UIElement* uiElement) {
	// for every ui batch
	for (int i = 0; i < _uiBatches.size(); i++) {
		// try and remove the element from the uiBatch
		if (_uiBatches[i].removeElement(uiElement)) {
			// if we removed one
			if (_uiBatches[i].isEmpty()) {
				// if the current ui batch is empty, remove it
				_uiBatches.erase(_uiBatches.begin() + i);
			}
			// then dont try to remove any more
			return;
		}
	}
}

void UIManager::optimize() {
	// we dont want duplicate batches
	_uiBatches.clear();

	for(int i = 0; i < _uiElements.size(); ++i) {
		// create a program based on our uiElement
		GLSLProgram* program = _programs[i]; // its worth noting _programs lines up with _uiElements
		
		// create a temporary batch to grab our programs with
		UIBatch temporaryBatch(program, _uiElements[i]);

		// we havent added any elements to this batch yet
		int addedElements = 0;

		// grab all elements (the current one and ones after it)
		for (int i2 = i; i2 < _uiElements.size(); ++i2) {
			// if an element's program matches the program we got earlier
			if (_programs[i2] == program) {
				// add it to our new batch
				temporaryBatch.addElement(_uiElements[i2]);
				// we just added an element
				++addedElements;
			} else {
				// this means that we dont have any elements after this matching the same program, so we can close this UIBatch
				break;
			}
		}
		
		// add our new uiBatch to our uiBatches list
		_uiBatches.push_back(temporaryBatch);

		// this ensures if we added more than one batch that we skip over the extra ones.
		i += addedElements - 1;
	}

}

void UIManager::draw() {
	for (int i = 0; i < _uiBatches.size(); ++i) {
		_uiBatches[i].draw(_camera, &_scale);
	}
}

void UIManager::setScale(float scale) {
	_scale = scale;
}

void UIManager::resolvePositions() {
	// we've changed our resolution, recalculate
	_recalculateOriginPinPositions();
	for (int i = 0; i < _uiElements.size(); ++i) {
		_uiElements[i]->resolvePosition(_camera, _calculatedOriginPinPositionsInScreenspacePtr, _scale);
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
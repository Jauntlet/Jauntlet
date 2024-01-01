#include "UIBatch.h"

using namespace Jauntlet;

UIBatch::UIBatch(GLSLProgram* program) {
	_program = program;
}

UIBatch::UIBatch(GLSLProgram* program, UIElement* UIElement) {
	_program = program;
	_UIElements.push_back(UIElement);
}

void UIBatch::addElement(UIElement* UIElement) {
    _UIElements.push_back(UIElement);
}

bool UIBatch::removeElement(UIElement* UIElement) {
	for (int i = 0; i < _UIElements.size(); i++) {
		if (_UIElements[i] == UIElement) {
			_UIElements.erase(_UIElements.begin() + i);
			return true;
		}
	}
	return false;
}
void UIBatch::clearBatch() {
	_UIElements.clear();
}

void UIBatch::draw(Camera2D* _camera, float* _scale) {

	GLSLProgram* storedProgram = GLSLProgram::currentProgram;

	_program->use();
	
	_camera->setActive();

	_spriteBatch.begin();

	for (int i = 0; i < _UIElements.size(); ++i) {
		if (_UIElements[i]->visible) {
			_UIElements[i]->draw(_camera, &_spriteBatch, *_scale);
		}
	}

	_spriteBatch.endAndRender();

	_program->unuse();

	// We store and then use the previously used program as to make sure that if the user decides to draw the UI while
	// another program is active, it doesn't deactivate their program and cause what the user may decide is "undefined"
	// behavior -xm
	if (storedProgram != nullptr) {
		storedProgram->use();
	}
}

void UIBatch::resolvePositions(Camera2D* camera, glm::vec2* resolvedPins, float scale) {
	for (int i = 0; i < _UIElements.size(); ++i) {
		_UIElements[i]->resolvePosition(camera, resolvedPins, scale);
	}
}
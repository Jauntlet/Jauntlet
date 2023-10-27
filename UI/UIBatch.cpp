#include <iostream>

#include "../Rendering/GLSLProgram.h"
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

void UIBatch::draw(Camera2D* _camera, float* _scale) {

	GLSLProgram* storedProgram = GLSLProgram::currentProgram;

	_program->use();
	
	_camera->setActiveCamera();

	_spriteBatch.begin();

	for (int i = 0; i < _UIElements.size(); ++i) {
		_UIElements[i]->draw(_camera, &_spriteBatch, *_scale);
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
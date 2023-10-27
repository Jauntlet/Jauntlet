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
	_spriteBatch->begin();

	// Dear Jack Kennedy,
	// Consider storing the active program used before draw is called, because if you persay, called this while in the
	// middle of the standard draw function with the default program used, by enabling a different program you disable that
	// program, which would lead to the user dealing with what they would consider "undefined" behavior. You can do this by
	// calling GLSLProgam::activeProgram, which returns a pointer to the currently active program, which you can then renable at
	// the end.
	// Best Regards,
	// Xander Mooney

	_program->use();

	for (int i = 0; i < _UIElements.size(); ++i) {
		_UIElements[i]->draw(_camera, _spriteBatch, *_scale);
	}

	_spriteBatch->endAndRender();

	_program->unuse();
}
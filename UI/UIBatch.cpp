#include "../Rendering/GLSLProgram.h"
#include "UIBatch.h"

using namespace Jauntlet;

UIBatch::UIBatch(GLuint programID) {
	_programID = programID;
}

UIBatch::UIBatch(GLuint programID, UIElement* UIElement) {
	_programID = programID;
	_UIElements.push_back(UIElement);
}

void UIBatch::addElement(UIElement* UIElement) {
    _UIElements.push_back(UIElement);
}

void UIBatch::draw(Camera2D* _camera, float* _scale) {
	_spriteBatch->begin();

	GLSLProgram::use(_programID);

	for (int i = 0; i < _UIElements.size(); ++i) {
		_UIElements[i]->draw(_camera, _spriteBatch, *_scale);
	}

	_spriteBatch->endAndRender();	
}
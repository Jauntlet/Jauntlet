#include "UIBatch.h"

using namespace Jauntlet;

UIBatch::UIBatch(GLuint programID) {
	_programID = programID;
}

UIBatch::UIBatch(GLuint programID, UIElement UIElement) {
	_programID = programID;
	_UIElements.push_back(UIElement);
}

void UIBatch::addElement(UIElement UIElement) {
    _UIElements.push_back(UIElement);
}

void UIBatch::draw(Camera2D* _camera, SpriteBatch* _spriteBatch, float* _scale) {
	_spriteBatch->begin();

	
}
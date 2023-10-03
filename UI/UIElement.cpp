#include <string>
#include <iostream>

#include "UIElement.h"

using namespace Jauntlet;

UIElement::UIElement() {
	// empty
}

void UIElement::draw(Camera2D* camera, SpriteBatch* spriteBatch, float scale) {
	std::cout << "wrong method! dont use a generic update statement like this" << std::endl;
}

void UIElement::resolvePosition(Camera2D* camera, glm::vec2 resolvedPins[], float scale) {
	_resolvedPosition = camera->convertScreenToWorld(*_position);
}
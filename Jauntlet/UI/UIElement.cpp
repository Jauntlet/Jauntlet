#include <string>
#include <iostream>

#include "UIElement.h"

using namespace Jauntlet;

UIElement::UIElement() {
	// empty
}

void UIElement::update(Camera2D camera, SpriteBatch spriteBatch, float scale) {
	std::cout << "wrong method! dont use a generic update statement like this" << std::endl;
}

void UIElement::fixResolution() {
	// cock
}
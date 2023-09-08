#include <string>

#include "UITextElement.h"

using namespace Jauntlet;

UITextElement::UITextElement(Camera2D* camera, std::string* text) {
	_camera = camera;
    _text = text;
}
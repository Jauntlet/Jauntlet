#include <string>
#include <iostream>
#include <glm/fwd.hpp>

#include "UIElement.h"
#include "UIManager.h"
#include "UITextElement.h"
//#include "../Rendering/SpriteFont.h"

using namespace Jauntlet;

UITextElement::UITextElement(SpriteFont* spriteFont, std::string* text, Color* color, glm::vec2* position) {
    _text = text;
    _spriteFont = spriteFont;
    _color = color;
    _position = position;
}

void UITextElement::draw(Camera2D* camera, SpriteBatch* spriteBatch, glm::vec2* scale) {
    _spriteFont->draw(*spriteBatch, *_text, _resolvedPostion, *scale, 0, *_color);
}
#include <string>

#include "UIManager.h"
#include "UITextElement.h"
#include "glm/fwd.hpp"

using namespace Jauntlet;

UITextElement::UITextElement(SpriteFont* spriteFont,  std::string* text, GLfloat tint[]) {
    _text = text;
    _spriteFont = spriteFont;
    _color = tint;
}

void UITextElement::update(Camera2D* camera, SpriteBatch& spriteBatch, float scale) {
    _spriteFont->draw(spriteBatch, *_text, _position, glm::vec2(scale), 0, _color);
}
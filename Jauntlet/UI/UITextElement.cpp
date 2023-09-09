#include <string>

#include "UIManager.h"
#include "UITextElement.h"
#include "glm/fwd.hpp"

using namespace Jauntlet;

UITextElement::UITextElement(SpriteFont* spriteFont,  std::string* text, Color* color) {
    _text = text;
    _spriteFont = spriteFont;
    _color = color;
}

void UITextElement::update(Camera2D* camera, SpriteBatch& spriteBatch, float scale) {
    _spriteFont->draw(spriteBatch, *_text, _position, glm::vec2(scale), 0, *_color); // there is absolutely no god -jk
}
#include <string>
#include <glm/fwd.hpp>

#include "UIElement.h"
#include "UIManager.h"
#include "UITextElement.h"

using namespace Jauntlet;

UITextElement::UITextElement(SpriteFont* spriteFont, std::string* text, Color* color, glm::vec2* position) {
    _text = text;
    _spriteFont = spriteFont;
    _color = color;
    _position = position;
}

void UITextElement::draw(Camera2D* camera, SpriteBatch* spriteBatch, float scale) {
    _spriteFont->draw(*spriteBatch, *_text, _resolvedPosition, glm::vec2(scale), 0, *_color);
}
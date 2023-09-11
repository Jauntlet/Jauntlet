#include <string>
#include <iostream>
#include <glm/fwd.hpp>

#include "UIElement.h"
#include "UIManager.h"
#include "UITextElement.h"

using namespace Jauntlet;

UITextElement::UITextElement(SpriteFont spriteFont,  std::string text, Color color, glm::vec2 _position) {
    _text = text;
    _spriteFont = spriteFont;
    _color = color;
}

void UITextElement::update(Camera2D camera, SpriteBatch spriteBatch, float scale) {
    _spriteFont.draw(spriteBatch, "bruh", glm::vec2(0), glm::vec2(scale), 0, Jauntlet::Color(1,1,1,1));
}
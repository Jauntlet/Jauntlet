#include "UITextElement.h"

using namespace Jauntlet;

UITextElement::UITextElement(TextRenderer* textRenderer, std::string* text, Color* color, glm::vec2* position) {
    _text = text;
    _textRenderer = textRenderer;
    _color = color;
    _position = position;
}

void UITextElement::draw(Camera2D* camera, SpriteBatch* spriteBatch, float scale) {
    _textRenderer->addText(*spriteBatch,*_text, _resolvedPosition, glm::vec2(scale), 0, *_color);
}
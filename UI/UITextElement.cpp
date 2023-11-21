#include "UITextElement.h"

using namespace Jauntlet;

UITextElement::UITextElement(TextRenderer* textRenderer, std::string* text, Color* color, glm::vec2* position, const float scale) {
    _text = text;
    _textRenderer = textRenderer;
    _color = color;
    _position = position;
    _scale = scale;
}

void UITextElement::draw(Camera2D* camera, SpriteBatch* spriteBatch, float scale) {
    _textRenderer->addText(*spriteBatch,*_text, _resolvedPosition, glm::vec2(scale * _scale), 0, *_color);
}
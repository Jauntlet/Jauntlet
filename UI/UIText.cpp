#include "UIText.h"

UIText::UIText(TextRenderer* textRenderer, std::string* text, Color* color, glm::vec2* position, const float scale) {
    _text = text;
    _textRenderer = textRenderer;
    _color = color;
    _position = position;
    _scale = scale;
}
UIText::UIText(TextRenderer* textRenderer, std::string* text, Color* color, glm::vec2* position, ORIGIN_PIN positionPinType, const float scale) {
    _text = text;
    _textRenderer = textRenderer;
    _color = color;
    _position = position;
    _scale = scale;
    _originPin = positionPinType;
}

void UIText::resolvePosition(Camera2D* camera, glm::vec2* resolvedPins, float scale) {

    glm::vec2 _size = _textRenderer->calculateTextSize(*_text, glm::vec2(_scale));
    glm::vec2 pin;
    switch (_originPin) {
        case ORIGIN_PIN::TOP_LEFT:
            _resolvedPosition = resolvedPins[(int)(_originPin)] + camera->convertScreenToWorld(glm::vec2(_position->x * scale, _position->y * scale));
            break;
        case ORIGIN_PIN::TOP:
            _resolvedPosition = resolvedPins[(int)(_originPin)] + camera->convertScreenToWorld(glm::vec2(_position->x * scale - ((_size.x * scale) * 0.5f), _position->y * scale));
            break;
        case ORIGIN_PIN::TOP_RIGHT:
            _resolvedPosition = resolvedPins[(int)(_originPin)] + camera->convertScreenToWorld(glm::vec2(_position->x * scale - _size.x * scale, _position->y * scale));
            break;
        case ORIGIN_PIN::RIGHT:
            _resolvedPosition = resolvedPins[(int)(_originPin)] + camera->convertScreenToWorld(glm::vec2(_position->x * scale - _size.x * scale, _position->y * scale));
            break;
        case ORIGIN_PIN::BOTTOM_RIGHT:
            _resolvedPosition = resolvedPins[(int)(_originPin)] + camera->convertScreenToWorld(glm::vec2(_position->x * scale - _size.x * scale, _position->y * scale - _size.y * scale));
            break;
        case ORIGIN_PIN::BOTTOM:
            _resolvedPosition = resolvedPins[(int)(_originPin)] + camera->convertScreenToWorld(glm::vec2(_position->x * scale - ((_size.x * scale ) * 0.5f),_position->y * scale - _size.y * scale));
            break;
        case ORIGIN_PIN::BOTTOM_LEFT:
            _resolvedPosition = resolvedPins[(int)(_originPin)] + camera->convertScreenToWorld(glm::vec2(_position->x * scale, _position->y * scale - _size.y * scale));
            break;
        case ORIGIN_PIN::LEFT:
            _resolvedPosition = resolvedPins[(int)(_originPin)] + camera->convertScreenToWorld(glm::vec2(_position->x * scale, _position->y * scale - ((_size.y * scale) * 0.5f)));
            break;
        case ORIGIN_PIN::CENTER:
            _resolvedPosition = resolvedPins[(int)(_originPin)] + camera->convertScreenToWorld(glm::vec2(_position->x * scale - ((_size.x * scale) * 0.5f), _position->y * scale - ((_size.y * scale) * 0.5f)));
            break;
    }
}

void UIText::draw(Camera2D* camera, SpriteBatch* spriteBatch, float scale) {
    _textRenderer->addText(*spriteBatch,*_text, _resolvedPosition, glm::vec2(scale * _scale), 0, *_color);
}
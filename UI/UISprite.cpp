#include "UISprite.h"

UISprite::UISprite() {
    //empty
}

UISprite::UISprite(unsigned int textureId, glm::vec2* position, glm::vec2 size, ORIGIN_PIN positionPinType) {
    _textureId = textureId;
    _originPin = positionPinType;
    _position = position;
    _size = size;
}

void UISprite::resolvePosition(Camera2D* camera, glm::vec2* resolvedPins, float scale) {
    switch (_originPin) {
        case ORIGIN_PIN::TOP_LEFT:
            _unresolvedPosition = glm::vec2(_position->x * scale, _position->y * scale + _size.y * scale);
            break;
        case ORIGIN_PIN::TOP:
            _unresolvedPosition = glm::vec2(_position->x * scale - ((_size.x * scale) * 0.5f), _position->y * scale + _size.y * scale);
            break;
        case ORIGIN_PIN::TOP_RIGHT:
            _unresolvedPosition = glm::vec2(_position->x * scale - _size.x * scale, _position->y * scale  + _size.y * scale);
            break;
        case ORIGIN_PIN::RIGHT:
            _unresolvedPosition = glm::vec2(_position->x * scale - _size.x * scale, _position->y * scale + ((_size.y * scale) * 0.5f));
            break;
        case ORIGIN_PIN::BOTTOM_RIGHT:
            _unresolvedPosition = glm::vec2(_position->x * scale - _size.x * scale, _position->y * scale);
            break;
        case ORIGIN_PIN::BOTTOM:
            _unresolvedPosition = glm::vec2(_position->x * scale - ((_size.x * scale ) * 0.5f), _position->y * scale);
            break;
        case ORIGIN_PIN::BOTTOM_LEFT:
            _unresolvedPosition = glm::vec2(_position->x * scale, _position->y * scale);
            break;
        case ORIGIN_PIN::LEFT:
            _unresolvedPosition = glm::vec2(_position->x * scale, _position->y * scale + ((_size.y * scale) * 0.5f));
            break;
        case ORIGIN_PIN::CENTER:
            _unresolvedPosition = glm::vec2(_position->x * scale - ((_size.x * scale) * 0.5f), _position->y * scale + ((_size.y * scale) * 0.5f));
            break;
    }

    _resolvedPosition = resolvedPins[(int)(_originPin)] + camera->convertScreenToWorld(_unresolvedPosition);
    _resolvedSize = _size * scale; // we will handle scaling later.
}

void UISprite::draw(Camera2D* camera, SpriteBatch* spriteBatch, float scale) {
    spriteBatch->draw({_resolvedPosition.x, _resolvedPosition.y, _resolvedSize.x, _resolvedSize.y}, _textureId);
}
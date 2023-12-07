#include <string>
#include <glm/fwd.hpp>

#include "UIButtonElement.h"

using namespace Jauntlet;

UIButtonElement::UIButtonElement() {
    //empty
}

UIButtonElement::UIButtonElement(InputManager* inputManager, std::function<void()> onClick, GLuint textureId, glm::vec2* position, glm::vec2 size, ORIGIN_PIN positionPinType) {
    _inputManager = inputManager;
    _onClick = onClick;
    _textureId = textureId;
    _originPin = positionPinType;
    _position = position;
    _size = size;
}

void UIButtonElement::resolvePosition(Camera2D* camera, glm::vec2* resolvedPins, float scale) {
    switch (_originPin) {
        case UIElement::ORIGIN_PIN::TOP_LEFT:
            _unresolvedPosition = glm::vec2(_position->x * scale, _position->y * scale + _size.y * scale);
            break;
        case UIElement::ORIGIN_PIN::TOP:
            _unresolvedPosition = glm::vec2(_position->x * scale - ((_size.x * scale) * 0.5f), _position->y * scale + _size.y * scale);
            break;
        case UIElement::ORIGIN_PIN::TOP_RIGHT:
            _unresolvedPosition = glm::vec2(_position->x * scale - _size.x * scale, _position->y * scale  + _size.y * scale);
            break;
        case UIElement::ORIGIN_PIN::RIGHT:
            _unresolvedPosition = glm::vec2(_position->x * scale - _size.x * scale, _position->y * scale + ((_size.y * scale) * 0.5f));
            break;
        case UIElement::ORIGIN_PIN::BOTTOM_RIGHT:
            _unresolvedPosition = glm::vec2(_position->x * scale - _size.x * scale, _position->y * scale);
            break;
        case UIElement::ORIGIN_PIN::BOTTOM:
            _unresolvedPosition = glm::vec2(_position->x * scale - ((_size.x * scale ) * 0.5f), _position->y * scale);
            break;
        case UIElement::ORIGIN_PIN::BOTTOM_LEFT:
            _unresolvedPosition = glm::vec2(_position->x * scale, _position->y * scale);
            break;
        case UIElement::ORIGIN_PIN::LEFT:
            _unresolvedPosition = glm::vec2(_position->x * scale, _position->y * scale + ((_size.y * scale) * 0.5f));
            break;
        case UIElement::ORIGIN_PIN::CENTER:
            _unresolvedPosition = glm::vec2(_position->x * scale - ((_size.x * scale) * 0.5f), _position->y * scale + ((_size.y * scale) * 0.5f));
            break;
    }

    _resolvedPosition = resolvedPins[(int)(_originPin)] + camera->convertScreenToWorld(_unresolvedPosition);
    _resolvedSize = _size * scale; // we will handle scaling later.
    
    _onScreenButtonVec4 = {
        _unresolvedPosition.x + resolvedPins[(int)(_originPin)].x,
        _unresolvedPosition.x + _size.x * scale + resolvedPins[(int)(_originPin)].x,
        _unresolvedPosition.y - resolvedPins[(int)(_originPin)].y,
        _unresolvedPosition.y - _size.y * scale - resolvedPins[(int)(_originPin)].y,
    };
}

void UIButtonElement::draw(Camera2D* camera, SpriteBatch* spriteBatch, float scale) {
    glm::vec2 mousecoords = _inputManager->getMouseCoords();

    _clicked = false;
    if (mousecoords.x >= _onScreenButtonVec4.x && mousecoords.y >= _onScreenButtonVec4.w && mousecoords.x <= _onScreenButtonVec4.y && mousecoords.y <= _onScreenButtonVec4.z) {
        if (_inputManager->isKeyPressed(SDL_BUTTON_LEFT)) {
            _onClick();
            _clicked = true;
            _wasPressed = true;
        } else if (_inputManager->isKeyDown(SDL_BUTTON_LEFT) && _wasPressed) {
            _clicked = true;
        }
    } else {
        _wasPressed = false;
    }
    
    spriteBatch->draw({_resolvedPosition.x, _resolvedPosition.y, _resolvedSize.x, _resolvedSize.y}, {_clicked * 0.5, 0, 0.5, 1}, _textureId);
}
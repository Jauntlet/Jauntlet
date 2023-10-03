#include <string>
#include <iostream>
#include <glm/fwd.hpp>

#include "SDL/SDL_mouse.h"
#include "UIElement.h"
#include "UIManager.h"
#include "UIButtonElement.h"

using namespace Jauntlet;

UIButtonElement::UIButtonElement(InputManager* inputManager, std::function<void()> onClick, GLuint textureId, glm::vec2* position, glm::vec2 size, ORIGIN_PIN positionPinType) {
    _inputManager = inputManager;
    _onClick = onClick;
    _textureId = textureId;
    _originPin = positionPinType;
    _position = position;
    _size = size;
}

void UIButtonElement::resolvePosition(Camera2D* camera, glm::vec2 resolvedPins[]) {
    
    switch (_originPin) {
        case UIElement::ORIGIN_PIN::TOP_LEFT:
            _resolvedPosition = glm::vec2(_position->x, _position->y + _size.y);
            break;
        case UIElement::ORIGIN_PIN::TOP:
            _resolvedPosition = glm::vec2(_position->x - (_size.x / 2), _position->y + _size.y);
            break;
        case UIElement::ORIGIN_PIN::TOP_RIGHT:
            _resolvedPosition = glm::vec2(_position->x - _size.x, _position->y  + _size.y);
            break;
        case UIElement::ORIGIN_PIN::RIGHT:
            _resolvedPosition = glm::vec2(_position->x - _size.x, _position->y + (_size.y / 2));
            break;
        case UIElement::ORIGIN_PIN::BOTTOM_RIGHT:
            _resolvedPosition = glm::vec2(_position->x - _size.x, _position->y);
            break;
        case UIElement::ORIGIN_PIN::BOTTOM:
            _resolvedPosition = glm::vec2(_position->x - (_size.x / 2), _position->y);
            break;
        case UIElement::ORIGIN_PIN::BOTTOM_LEFT:
            _resolvedPosition = glm::vec2(_position->x, _position->y);
            break;
        case UIElement::ORIGIN_PIN::LEFT:
            _resolvedPosition = glm::vec2(_position->x, _position->y + (_size.y / 2));
            break;
        case UIElement::ORIGIN_PIN::CENTER:
            _resolvedPosition = glm::vec2(_position->x - (_size.x / 2), _position->y + (_size.y / 2));
            break;
    }

    std::cout << "pos:  " << _position->x << ", " << _position->y << std::endl;
    std::cout << "size: " << _size.x << ", " << _size.y << std::endl;

    std::cout << "BEFORE SCALING: " << _resolvedPosition.x << ", " << _resolvedPosition.y << std::endl;

    _resolvedPosition *= 2.0f;

    std::cout << "AFTER SCALING:  " << _resolvedPosition.x << ", " << _resolvedPosition.y << std::endl;
    
    _resolvedPosition = resolvedPins[(int)(_originPin)] + camera->convertScreenToWorld(_resolvedPosition);
    _resolvedSize = _size; // we will handle scaling later.
    
    _onScreenButtonVec4 = {
        _position->x + resolvedPins[(int)(_originPin)].x,
        _position->x + _size.x + resolvedPins[(int)(_originPin)].x,
        _position->y - resolvedPins[(int)(_originPin)].y,
        _position->y - _size.y - resolvedPins[(int)(_originPin)].y,
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
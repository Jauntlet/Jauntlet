#include <string>
#include <iostream>
#include <glm/fwd.hpp>

#include "SDL/SDL_mouse.h"
#include "UIElement.h"
#include "UIManager.h"
#include "UIButtonElement.h"
//#include "../Rendering/SpriteFont.h"

using namespace Jauntlet;

UIButtonElement::UIButtonElement(InputManager* inputManager, std::function<void()> onClick, GLuint textureId, glm::vec2* position, glm::vec2 size, ORIGIN_PIN positionPinType) {
    _inputManager = inputManager;
    _onClick = onClick;
    _textureId = textureId;

    _originPin = positionPinType;

    switch (positionPinType) {
        case UIElement::ORIGIN_PIN::TOP_LEFT:
            _position = new glm::vec2(position->x, position->y + size.y);
            break;
        case UIElement::ORIGIN_PIN::TOP:
            _position = new glm::vec2(position->x - (size.x / 2), position->y + size.y);
            break;
        case UIElement::ORIGIN_PIN::TOP_RIGHT:
            _position = new glm::vec2(position->x - size.x, position->y  + size.y);
            break;
        case UIElement::ORIGIN_PIN::RIGHT:
            _position = new glm::vec2(position->x - size.x, position->y + (size.y / 2));
            break;
        case UIElement::ORIGIN_PIN::BOTTOM_RIGHT:
            _position = new glm::vec2(position->x - size.x, position->y);
            break;
        case UIElement::ORIGIN_PIN::BOTTOM:
            _position = new glm::vec2(position->x - (size.x / 2), position->y);
            break;
        case UIElement::ORIGIN_PIN::BOTTOM_LEFT:
            _position = new glm::vec2(position->x, position->y);
            break;
        case UIElement::ORIGIN_PIN::LEFT:
            _position = new glm::vec2(position->x, position->y + (size.y / 2));
            break;
        case UIElement::ORIGIN_PIN::CENTER:
            _position = new glm::vec2(position->x - (size.x / 2), position->y + (size.y / 2));
            break;
    }

    _size = size;
}

void UIButtonElement::resolvePosition(Camera2D* camera, glm::vec2 resolvedPins[]) {
    _resolvedPostion = camera->convertScreenToWorld(*_position);
    _resolvedPostion += resolvedPins[(int)(_originPin)];
    _resolvedSize = _size; // we will handle scaling later.
    
    _onScreenButtonVec4 = {
        _position->x + resolvedPins[(int)(_originPin)].x,
        _position->x + _size.x + resolvedPins[(int)(_originPin)].x,
        _position->y - resolvedPins[(int)(_originPin)].y,
        _position->y - _size.y - resolvedPins[(int)(_originPin)].y,
    };
}

void UIButtonElement::draw(Camera2D* camera, SpriteBatch* spriteBatch, glm::vec2* scale) {
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
    
    spriteBatch->draw({_resolvedPostion.x, _resolvedPostion.y, _resolvedSize.x, _resolvedSize.y}, {_clicked * 0.5, 0, 0.5, 1}, _textureId);
}
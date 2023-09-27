#include <string>
#include <iostream>
#include <glm/fwd.hpp>

#include "SDL/SDL_mouse.h"
#include "UIElement.h"
#include "UIManager.h"
#include "UIButtonElement.h"
//#include "../Rendering/SpriteFont.h"

using namespace Jauntlet;

UIButtonElement::UIButtonElement(InputManager* inputManager, std::function<void()> onClick, GLuint textureId, glm::vec2* position, glm::vec2* size, PIN_POSITION positionPinType) {
    _inputManager = inputManager;
    _onClick = onClick;
    _textureId = textureId;

    switch (positionPinType) {
        case UIElement::PIN_POSITION::TOP_RIGHT:
            _position = new glm::vec2(position->x - size->x, position->y  + size->y);
            break;
        case UIElement::PIN_POSITION::BOTTOM_RIGHT:
            _position = new glm::vec2(position->x - size->x, position->y);
            break;
        case UIElement::PIN_POSITION::BOTTOM_LEFT:
            _position = new glm::vec2(position->x, position->y);
            break;
        case UIElement::PIN_POSITION::CENTER:
            _position = new glm::vec2(position->x - (size->x / 2), position->y + (size->y / 2));
            break;
        case UIElement::PIN_POSITION::TOP_LEFT:
        default: // TOP_LEFT or anything else
            _position = new glm::vec2(position->x, position->y + size->y);
            break;
    }

    _size = size;
}

void UIButtonElement::resolvePosition(Camera2D* camera) {
    _resolvedPostion = camera->convertScreenToWorld(*_position);
    _resolvedSize = *_size;
}

void UIButtonElement::draw(Camera2D* camera, SpriteBatch* spriteBatch, glm::vec2* scale) {
    
    glm::vec2 mousecoords = _inputManager->getMouseCoords();

    _clicked = false;
    if (mousecoords.x >= _position->x && mousecoords.y >= _position->y - _size->y && mousecoords.x <= _position->x + _size->x && mousecoords.y <= _position->y) {
        if (_inputManager->isKeyPressed(SDL_BUTTON_LEFT)) {
            _onClick();
            _clicked = true;
        } else if (_inputManager->isKeyDown(SDL_BUTTON_LEFT)) {
            _clicked = true;
        }
    }
    
    spriteBatch->draw({_resolvedPostion.x, _resolvedPostion.y, _resolvedSize.x, _resolvedSize.y}, {_clicked ? 0.5 : 0, 0, 0.5, 1}, _textureId);
}
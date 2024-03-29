#include "UIButtonToggleable.h"

UIButtonToggleable::UIButtonToggleable() {
    // empty
}

UIButtonToggleable::UIButtonToggleable(InputManager* inputManager, std::function<void()> onClick, GLuint textureId, glm::vec2* position, glm::vec2 size, ORIGIN_PIN positionPinType) {
    _inputManager = inputManager;
    _onClick = onClick;
    _textureId = textureId;
    _originPin = positionPinType;
    _position = position;
    _size = size;
    _clicked = false;
}

void UIButtonToggleable::draw(Camera2D* camera, SpriteBatch* spriteBatch, float scale) {
    glm::vec2 mousecoords = _inputManager->getMouseCoords();

    if (_inputManager->isKeyPressed(SDL_BUTTON_LEFT) && mousecoords.x >= _onScreenButtonVec4.x && mousecoords.y >= _onScreenButtonVec4.w && mousecoords.x <= _onScreenButtonVec4.y && mousecoords.y <= _onScreenButtonVec4.z) {
        _onClick();
        _clicked = !_clicked;
    }
    
    spriteBatch->draw({_resolvedPosition.x, _resolvedPosition.y, _resolvedSize.x, _resolvedSize.y}, {_clicked * 0.5, 0, 0.5, 1}, _textureId);
}

void UIButtonToggleable::setClicked(bool clicked) {
    _clicked = clicked;
}

bool UIButtonToggleable::isClicked() {
    return _clicked;
}
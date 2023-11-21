#include "UISpriteAnimatedElement.h"

using namespace Jauntlet;

UISpriteAnimatedElement::UISpriteAnimatedElement() {
    //empty
}

UISpriteAnimatedElement::UISpriteAnimatedElement(GLuint textureId, glm::vec2* position, glm::vec2 size, ORIGIN_PIN positionPinType, Animation* animation) {
    _textureId = textureId;
    _originPin = positionPinType;
    _position = position;
    _size = size;
    _animation = animation;
}

void UISpriteAnimatedElement::draw(Camera2D* camera, SpriteBatch* spriteBatch, float scale) {
    spriteBatch->draw({_resolvedPosition.x, _resolvedPosition.y, _resolvedSize.x, _resolvedSize.y},_animation->getUV(), _textureId);
}
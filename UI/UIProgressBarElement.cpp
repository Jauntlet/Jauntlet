/* Purpose: UI Element and Progress Bar parity
 * Main Contributer(s): Xander Mooney
 */
#include "UIProgressBarElement.h"

using namespace Jauntlet;

UIProgressBarElement::UIProgressBarElement(const std::string& texture1, const glm::vec4& texture1UV, const std::string& texture2, const glm::vec4& texture2UV, const glm::vec4& dest, UIElement::ORIGIN_PIN pinType) : 
	ProgressBar(texture1, texture1UV, texture2, texture2UV, dest)
{
    _originPin = pinType;
}
UIProgressBarElement::UIProgressBarElement(const std::string& texture, const glm::vec4& texture1UV, const glm::vec4& texture2UV, const glm::vec4& dest, UIElement::ORIGIN_PIN pinType) :
	ProgressBar(texture, texture1UV, texture2UV, dest)
{
    _originPin = pinType;
}

void UIProgressBarElement::draw(Camera2D* camera, SpriteBatch* spriteBatch, float scale) { 
    if (progress != 0) {
        spriteBatch->draw(glm::vec4(_resolvedPosition, _resolvedSize.x * progress, _resolvedSize.y), glm::vec4(_UV1.x, _UV1.y, _UV1.z * progress, _UV1.w), _fullTexture);
    }
    if (progress != 1) {
        spriteBatch->draw(glm::vec4(_resolvedPosition.x + _resolvedSize.x * progress, _resolvedPosition.y, _resolvedSize.x * (1 - progress), _resolvedSize.y), glm::vec4(_UV2.x + _UV2.z * progress, _UV2.y, _UV2.z * (1 - progress), _UV1.w), _emptyTexture);
    }
};
void UIProgressBarElement::resolvePosition(Camera2D* camera, glm::vec2* resolvedPins, float scale) {
    glm::vec2 _unresolvedPosition;

    switch (_originPin) {
    case UIElement::ORIGIN_PIN::TOP_LEFT:
        _unresolvedPosition = glm::vec2(position.x * scale, position.y * scale + _size.y * scale);
        break;
    case UIElement::ORIGIN_PIN::TOP:
        _unresolvedPosition = glm::vec2(position.x * scale - ((_size.x * scale) * 0.5f), position.y * scale + _size.y * scale);
        break;
    case UIElement::ORIGIN_PIN::TOP_RIGHT:
        _unresolvedPosition = glm::vec2(position.x * scale - _size.x * scale, position.y * scale + _size.y * scale);
        break;
    case UIElement::ORIGIN_PIN::RIGHT:
        _unresolvedPosition = glm::vec2(position.x * scale - _size.x * scale, position.y * scale + ((_size.y * scale) * 0.5f));
        break;
    case UIElement::ORIGIN_PIN::BOTTOM_RIGHT:
        _unresolvedPosition = glm::vec2(position.x * scale - _size.x * scale, position.y * scale);
        break;
    case UIElement::ORIGIN_PIN::BOTTOM:
        _unresolvedPosition = glm::vec2(position.x * scale - ((_size.x * scale) * 0.5f), position.y * scale);
        break;
    case UIElement::ORIGIN_PIN::BOTTOM_LEFT:
        _unresolvedPosition = glm::vec2(position.x * scale, position.y * scale);
        break;
    case UIElement::ORIGIN_PIN::LEFT:
        _unresolvedPosition = glm::vec2(position.x * scale, position.y * scale + ((_size.y * scale) * 0.5f));
        break;
    case UIElement::ORIGIN_PIN::CENTER:
        _unresolvedPosition = glm::vec2(position.x * scale - ((_size.x * scale) * 0.5f), position.y * scale + ((_size.y * scale) * 0.5f));
        break;
    }

    _resolvedPosition = resolvedPins[(int)(_originPin)] + camera->convertScreenToWorld(_unresolvedPosition);
    _resolvedSize = _size * scale; // we will handle scaling later.
}
/* Purpose: Creates a static sprite on screen, managable through the ui manager.
 * Main Contributer(s): Jack Kennedy
 */
#pragma once

#include "UISprite.h"
#include "../Rendering/Animation/SpriteAnimation.h"

class UISpriteAnimated : public UISprite
{
public:
	UISpriteAnimated();
	UISpriteAnimated(unsigned int textureId, glm::vec2* position, glm::vec2 size, ORIGIN_PIN positionPinType, SpriteAnimation* animation);
	void resolvePosition(Camera2D* camera, glm::vec2* resolvedPins, float scale) override;
       void draw(Camera2D* camera, SpriteBatch* spriteBatch, float scale) override;
   protected:
	std::string* _text;
       Color* _color;
	glm::vec2 _size;
	glm::vec2 _resolvedSize;
	unsigned int _textureId;
	glm::vec2 _unresolvedPosition;
	SpriteAnimation* _animation;
};
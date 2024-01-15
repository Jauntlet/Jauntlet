/* Purpose: Creates a static sprite on screen, managable through the ui manager.
 * Main Contributer(s): Jack Kennedy
 */
#pragma once

#include "UISpriteElement.h"
#include "../Rendering/Animation/Animation.h"

namespace Jauntlet {
	class UISpriteAnimatedElement : public UISpriteElement
	{
	public:
		UISpriteAnimatedElement();
		UISpriteAnimatedElement(unsigned int textureId, glm::vec2* position, glm::vec2 size, ORIGIN_PIN positionPinType, Animation* animation);
		void resolvePosition(Camera2D* camera, glm::vec2* resolvedPins, float scale) override;
        void draw(Camera2D* camera, SpriteBatch* spriteBatch, float scale) override;
    protected:
		std::string* _text;
        Color* _color;
		glm::vec2 _size;
		glm::vec2 _resolvedSize;
		unsigned int _textureId;
		glm::vec2 _unresolvedPosition;
		Animation* _animation;
	};
}
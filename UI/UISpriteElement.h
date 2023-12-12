/* Purpose: Creates a static sprite on screen, managable through the ui manager.
 * Main Contributer(s): Jack Kennedy
 */
#pragma once
#include "UIElement.h"

namespace Jauntlet {
	class UISpriteElement : public UIElement
	{
	public:
		UISpriteElement();
		UISpriteElement(GLuint textureId, glm::vec2* position, glm::vec2 size, ORIGIN_PIN positionPinType);
        virtual void draw(Camera2D* camera, SpriteBatch* spriteBatch, float scale) override;
		virtual void resolvePosition(Camera2D* camera, glm::vec2* resolvedPins, float scale) override;
    private:
        Color* _color;
		glm::vec2 _size;
		glm::vec2 _resolvedSize;
		GLuint _textureId;
		glm::vec2 _unresolvedPosition;
	};
}
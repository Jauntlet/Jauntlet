#pragma once

#include <functional>
#include <glm/fwd.hpp>
#include <glm/glm.hpp>
#include <string>

#include "../InputManager.h"
#include "UIElement.h"

namespace Jauntlet {
	class UISpriteElement : public UIElement
	{
	public:
		UISpriteElement();
		UISpriteElement(GLuint textureId, glm::vec2* position, glm::vec2 size, ORIGIN_PIN positionPinType);
        void draw(Camera2D* camera, SpriteBatch* spriteBatch, float scale) override;
		void resolvePosition(Camera2D* camera, glm::vec2* resolvedPins, float scale) override;
    private:
		std::string* _text;
        Color* _color;
		glm::vec2 _size;
		glm::vec2 _resolvedSize;
		GLuint _textureId;
		glm::vec2 _unresolvedPosition;
	};
}
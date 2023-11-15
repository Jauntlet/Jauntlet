/* Purpose: Creates a UI element on screen that takes a font and text and outputs real rendered text on screen.
 * Main Contributer(s): Jack Kennedy
 */

#pragma once

#include <glm/glm.hpp>
#include <string>

#include "UIElement.h"
#include "../Rendering/TextRenderer.h"

namespace Jauntlet {
	class UITextElement : public UIElement
	{
	public:
		//constructor with _position;
		UITextElement(TextRenderer* spriteFont, std::string* text, Color* color, glm::vec2* _position);
        void draw(Camera2D* camera, SpriteBatch* spriteBatch, float scale);
    private:
		std::string* _text;
        TextRenderer* _textRenderer;
        Color* _color;
	};
}
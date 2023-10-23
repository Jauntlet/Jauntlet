/* Purpose: Creates a UI element on screen that takes a font and text and outputs real rendered text on screen.
 * Main Contributer(s): Jack Kennedy
 */

#pragma once

#include <glm/glm.hpp>
#include <string>

#include "UIElement.h"
#include "../Rendering/SpriteFont.h"

namespace Jauntlet {
	class UITextElement : public UIElement
	{
	public:
		//constructor with _position;
		UITextElement(SpriteFont* spriteFont, std::string* text, Color* color, glm::vec2* _position);
        void draw(Camera2D* camera, SpriteBatch* spriteBatch, float scale) override;
    private:
		std::string* _text;
        SpriteFont* _spriteFont;
        Color* _color;
	};
}
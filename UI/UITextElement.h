#pragma once

#include <glm/glm.hpp>
#include <string>

#include "../Camera2D.h"
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
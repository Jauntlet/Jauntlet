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
		UITextElement(SpriteFont* spriteFont,  std::string* text, Color* color);
        void update(Camera2D* camera, SpriteBatch* spriteBatch, float scale); // wassup youtube today in minecraft we'll be getting spread wide open by c++ -jk
        // THE FUCKING &&&??????? AFTER THE TYPE????
    private:
		std::string* _text;
        SpriteFont* _spriteFont;
        Color* _color;
	};
}
#pragma once

#include <glm/glm.hpp>
#include <string>

#include "../Camera2D.h"
#include "UIElement.h"
#include "../Rendering/SpriteFont.h"

namespace Jauntlet {
	class UITextElement : UIElement
	{
	public:
		//constructor with _position;
		UITextElement(SpriteFont* spriteFont,  std::string* text, float tint[]);
		void update(Camera2D* camera, SpriteBatch& spriteBatch, float scale);
	private:
		std::string* _text;
        SpriteFont* _spriteFont;
        GLfloat* _color;
	};
}


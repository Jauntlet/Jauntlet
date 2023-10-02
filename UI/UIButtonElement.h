#pragma once

#include <functional>
#include <glm/fwd.hpp>
#include <glm/glm.hpp>
#include <string>

#include "../Camera2D.h"
#include "../InputManager.h"
#include "../Rendering/SpriteFont.h"
#include "UIElement.h"

namespace Jauntlet {
	class UIButtonElement : public UIElement
	{
	public:
		//constructor with _position;
		UIButtonElement(InputManager* inputManager, std::function<void()> onClick, GLuint textureId, glm::vec2 position, glm::vec2 size, ORIGIN_PIN positionPinType);
        void draw(Camera2D* camera, SpriteBatch* spriteBatch, glm::vec2* scale) override;
		void resolvePosition(Camera2D* camera, glm::vec2 resolvedPins[]) override;
		void click() { _onClick(); };
    private:
		std::string* _text;
        SpriteFont* _spriteFont;
        Color* _color;
		std::function<void()> _onClick;
		glm::vec2 _size;
		glm::vec2 _resolvedSize;
		GLuint _textureId;
		bool _clicked;
		bool _wasPressed = false;
		InputManager* _inputManager;
	};
}
#pragma once

#include <functional>
#include <glm/fwd.hpp>
#include <glm/glm.hpp>
#include <string>

#include "../InputManager.h"
#include "UIElement.h"

namespace Jauntlet {
	class UIButtonElement : public UIElement
	{
	public:
		UIButtonElement();
		UIButtonElement(InputManager* inputManager, std::function<void()> onClick, GLuint textureId, glm::vec2* position, glm::vec2 size, ORIGIN_PIN positionPinType);
        void draw(Camera2D* camera, SpriteBatch* spriteBatch, float scale) override;
		void resolvePosition(Camera2D* camera, glm::vec2 resolvedPins[], float scale) override;
		void click() { _onClick(); };
    private:
		std::string* _text;
        Color* _color;
		std::function<void()> _onClick;
		glm::vec2 _size;
		glm::vec2 _resolvedSize;
		GLuint _textureId;
		bool _clicked;
		bool _wasPressed = false;
		InputManager* _inputManager;
		glm::vec4 _onScreenButtonVec4;
		glm::vec2 _unresolvedPosition;
	};
}
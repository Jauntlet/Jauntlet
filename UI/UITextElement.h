/* Purpose: Creates a UI element on screen that takes a font and text and outputs real rendered text on screen.
 * Main Contributer(s): Jack Kennedy
 */

#pragma once
#include "UIElement.h"
#include "../Rendering/TextRenderer.h"

namespace Jauntlet {
	class UITextElement : public UIElement
	{
	public:
		UITextElement(TextRenderer* textRenderer, std::string* text, Color* color, glm::vec2* position, const float scale = 1.0f);
		UITextElement(TextRenderer* textRenderer, std::string* text, Color* color, glm::vec2* position, ORIGIN_PIN positionPinType, const float scale = 1.0f);

        void draw(Camera2D* camera, SpriteBatch* spriteBatch, float scale) override;
		void resolvePosition(Camera2D* camera, glm::vec2* resolvedPins, float scale) override;
    private:
		std::string* _text;
        TextRenderer* _textRenderer;
        Color* _color;
		float _scale;
	};
}
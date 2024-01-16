/* Purpose: Creates a clickable button on screen that runs a methon on click
 * Main Contributer(s): Jack Kennedy
 */

#pragma once

#include <functional>
#include <glm/ext/vector_float4.hpp>
#include <glm/ext/vector_float2.hpp>
#include <string>

#include "../Inputs/InputManager.h"
#include "UIElement.h"

class UIButton : public UIElement
{
public:
	UIButton();
	UIButton(InputManager* inputManager, std::function<void()> onClick, GLuint textureId, glm::vec2* position, glm::vec2 size, ORIGIN_PIN positionPinType);
       void draw(Camera2D* camera, SpriteBatch* spriteBatch, float scale) override;
	void resolvePosition(Camera2D* camera, glm::vec2* resolvedPins, float scale) override;
	void click() { _onClick(); };

	// Returns true if the button was pressed this frame
	bool isPressed();
	// Returns true if the button is currently pressed down
	bool isDown();
   protected:
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
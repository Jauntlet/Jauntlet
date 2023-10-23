/* Purpose: Creates a button that flicks "on" and "off", like a switch.
 * identical to UIButtonElement in functionality.
 * Main Contributer(s): Jack Kennedy
 */

#pragma once

#include "UIButtonElement.h"

namespace Jauntlet {
	class UIButtonToggleableElement : public UIButtonElement
	{
	public:
		UIButtonToggleableElement();
		UIButtonToggleableElement(InputManager* inputManager, std::function<void()> onClick, GLuint textureId, glm::vec2* position, glm::vec2 size, ORIGIN_PIN positionPinType);
        //constructor with _position;
		void draw(Camera2D* camera, SpriteBatch* spriteBatch, float scale) override;
    private:
		
	};
}
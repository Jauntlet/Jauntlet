#pragma once

#include <functional>
#include <glm/fwd.hpp>
#include <glm/glm.hpp>
#include <string>

#include "../Camera2D.h"
#include "../InputManager.h"
#include "../Rendering/SpriteFont.h"
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
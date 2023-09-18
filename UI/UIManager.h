#pragma once

#include <glm/glm.hpp>
#include <vector>

#include "../Camera2D.h"
#include "../Errors.h"
#include "UIElement.h"
#include "../Rendering/SpriteBatch.h"

namespace Jauntlet {
	class UIManager
	{
	public:
		//constructors;
		UIManager();
		UIManager(Camera2D* camera, SpriteBatch* spriteBatch);
		
		// update all ui elements to fit the resolution
		void addElement(UIElement* uiElement);
		// draw all hud elements
		void draw();
		// set the hud scale
		void setScale(float scale);
		// fix all hud elements after resize
		void resolvePositions();
	private:
		Camera2D* _camera;
		SpriteBatch* _spriteBatch;
		std::vector<UIElement*> _uiElements;
		glm::vec2* _scale;
	};
}


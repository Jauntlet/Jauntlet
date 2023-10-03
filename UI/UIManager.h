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
		UIManager(Camera2D* camera);
		
		// update all ui elements to fit the resolution
		void addElement(UIElement* uiElement);
		// draw all hud elements
		void draw();
		// set the hud scale
		void setScale(float scale);
		// set the hud scale with x and y scaling seperately
		void setScale(glm::vec2 scale);
		// fix all hud elements after resize
		void resolvePositions();
	private:
		Camera2D* _camera;
		SpriteBatch* _spriteBatch;
		std::vector<UIElement*> _uiElements;
		float _scale;

		// holy fucking shit i wish i could make this a const -jk
		glm::vec2 ORIGIN_PIN_POSITIONS[9] = {glm::vec2(0.0f,0.0f),glm::vec2(0.5f,0.0f),glm::vec2(1.0f,0.0f),glm::vec2(1.0f,0.5f),glm::vec2(1.0f,1.0f),glm::vec2(0.5f,1.0f),glm::vec2(0.0f,1.0f),glm::vec2(0.0f,0.5f),glm::vec2(0.5f,0.5f)};

		// im going to name my baby this -jk
		glm::vec2 _calculatedOriginPinPositionsInScreenspace[9];

		// recalculate the origin pin positions.
		void _recalculateOriginPinPositions();
	};
}


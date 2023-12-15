/* Purpose: Manages UIElements. Can draw, move, scale, and update given elements.
 * Main Contributer(s): Jack Kennedy
 */

#pragma once

#include <glm/ext/vector_float2.hpp>
#include <vector>

#include "UIBatch.h"

namespace Jauntlet {
	class UIManager
	{
	public:
		UIManager(Camera2D* camera);
		// update all ui elements to fit the resolution
		void addElement(UIElement* uiElement, GLSLProgram* program);
		// remove specified element from the UI
		void removeElement(UIElement* uiElement);
		// remove all elements from the UI
		void removeAllElements();
		// draw all hud elements
		void draw();
		// set the hud scale
		void setScale(float scale);
		// set the hud scale with x and y scaling seperately
		void setScale(glm::vec2 scale);
		// fix all hud elements after resize
		void resolvePositions();
		// orders all hud elements into batches before drawing
		void optimize();
	private:
		Camera2D* _camera;
		SpriteBatch _spriteBatch;
		std::vector<UIElement*> _uiElements;
		std::vector<GLSLProgram*> _programs;
		std::vector<UIBatch> _uiBatches;
		float _scale;

		// holy fucking shit i wish i could make this a const -jk
		glm::vec2 ORIGIN_PIN_POSITIONS[9] = {glm::vec2(0.0f,0.0f),glm::vec2(0.5f,0.0f),glm::vec2(1.0f,0.0f),glm::vec2(1.0f,0.5f),glm::vec2(1.0f,1.0f),glm::vec2(0.5f,1.0f),glm::vec2(0.0f,1.0f),glm::vec2(0.0f,0.5f),glm::vec2(0.5f,0.5f)};

		// im going to name my baby this -jk
		glm::vec2 _calculatedOriginPinPositionsInScreenspace[9];

		glm::vec2* _calculatedOriginPinPositionsInScreenspacePtr = _calculatedOriginPinPositionsInScreenspace;

		// recalculate the origin pin positions.
		void _recalculateOriginPinPositions();
	};
}


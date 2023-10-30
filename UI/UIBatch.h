/* Purpose: Batches of UIElements that the UIManager draws
 * Main Contributer(s): Jack Kennedy
 */

#pragma once

#include <glm/glm.hpp>
#include <vector>

#include "UIElement.h"
#include "../Camera2D.h"


namespace Jauntlet {
	class GLSLProgram;

	class UIBatch
	{
	public:
        // create a ui batch without an element
		UIBatch(GLSLProgram* program);
        // create a ui batch with an element
		UIBatch(GLSLProgram* program, UIElement* UIElement);
        // add an element to the ui batch
		void addElement(UIElement* UIElement);
		// draw
		void draw(Camera2D* _camera, float* _scale);
		// resolve positions
		void resolvePositions(Camera2D* camera, glm::vec2* resolvedPins, float scale);
	private:
		GLSLProgram* _program = nullptr;
		std::vector<UIElement*> _UIElements;

		SpriteBatch _spriteBatch;
	};
}
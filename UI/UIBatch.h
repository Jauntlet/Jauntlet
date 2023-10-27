/* Purpose: Batches of UIElements that the UIManager draws
 * Main Contributer(s): Jack Kennedy
 */

#pragma once

#include <glm/glm.hpp>
#include <vector>

#include "UIElement.h"
#include "../Camera2D.h"
#include "../Rendering/SpriteBatch.h"

namespace Jauntlet {
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
	private:
		GLSLProgram* _program;
		std::vector<UIElement*> _UIElements;

		SpriteBatch* _spriteBatch;
	};
}
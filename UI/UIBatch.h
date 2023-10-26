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
		UIBatch(GLuint programID);
        // create a ui batch with an element
		UIBatch(GLuint programID, UIElement UIElement);
        // add an element to the ui batch
		void addElement(UIElement UIElement);
		// draw
		void draw(Camera2D* _camera, SpriteBatch* _spriteBatch, float* _scale);
	private:
		GLuint _programID;
		std::vector<UIElement> _UIElements;
	};
}
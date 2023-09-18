#pragma once

#include <glm/glm.hpp>
#include <string>

#include "../Camera2D.h"
#include "glm/fwd.hpp"
#include "../Rendering/SpriteBatch.h"

namespace Jauntlet {
	class UIElement
	{
	public:
		enum ELEMENT_TYPE {TEXT, SPRITE, BAR}; // TODO: delete this shit -jk
		UIElement();
		virtual void draw(Camera2D* camera, SpriteBatch* spriteBatch, glm::vec2* scale);
		void resolvePosition(Camera2D* camera);
	protected:
		Camera2D* _camera;
		glm::vec2* _position;
		glm::vec2 _resolvedPostion;
	};
}


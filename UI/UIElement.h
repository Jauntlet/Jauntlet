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
		enum class PIN_POSITION {
			TOP_LEFT,
			TOP_RIGHT,
			BOTTOM_LEFT,
			BOTTOM_RIGHT,
			CENTER
		};
		UIElement();
		virtual void draw(Camera2D* camera, SpriteBatch* spriteBatch, glm::vec2* scale);
		virtual void resolvePosition(Camera2D* camera);
	protected:
		Camera2D* _camera;
		glm::vec2* _position;
		glm::vec2 _resolvedPostion;
	};
}


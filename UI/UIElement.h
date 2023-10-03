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
		enum class ORIGIN_PIN {
			TOP_LEFT = 0,
			TOP = 1,
			TOP_RIGHT = 2,
			RIGHT = 3,
			BOTTOM_RIGHT = 4,
			BOTTOM = 5,
			BOTTOM_LEFT = 6,
			LEFT = 7,
			CENTER = 8
		};

		UIElement();

		virtual void draw(Camera2D* camera, SpriteBatch* spriteBatch, float scale);
		virtual void resolvePosition(Camera2D* camera, glm::vec2 resolvedPins[], float scale);
		
		bool visible = true;
	protected:
		Camera2D* _camera;
		glm::vec2* _position;
		glm::vec2 _resolvedPosition;
		glm::vec2 _resolvedOrigin;
		ORIGIN_PIN _originPin = ORIGIN_PIN::TOP_LEFT;
	};
}


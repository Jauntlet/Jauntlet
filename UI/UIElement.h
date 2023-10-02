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
			TOP_LEFT,
			TOP,
			TOP_RIGHT,
			RIGHT,
			BOTTOM_RIGHT,
			BOTTOM,
			BOTTOM_LEFT,
			LEFT,
			CENTER
		};

		UIElement();

		virtual void draw(Camera2D* camera, SpriteBatch* spriteBatch, glm::vec2* scale);
		virtual void resolvePosition(Camera2D* camera, glm::vec2 resolvedPins[]);
		
		bool visible = true;
	protected:
		Camera2D* _camera;
		glm::vec2* _position;
		glm::vec2 _resolvedPostion;
		glm::vec2 _resolvedOrigin;
		ORIGIN_PIN _originPin = ORIGIN_PIN::TOP_LEFT;
	};
}


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
		enum ELEMENT_TYPE {TEXT, SPRITE, BAR};
		UIElement();
		//constructor with _position; and camera
		UIElement(Camera2D* camera);
		//Update UI when camera is reized
		virtual void update(Camera2D* camera, SpriteBatch& spriteBatch, float scale);
		
		virtual void fixResolution();
	protected:
		Camera2D* _camera;
		glm::vec2 _position;
	};
}


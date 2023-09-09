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
		//constructor with _position; and camera
		UIElement(Camera2D* camera);
		//Update UI when camera is reized
		void update(Camera2D* camera, SpriteBatch& spriteBatch, float scale);
		// crazy how this is both not defined in UIElement.cpp nor does it even have different arguments than its decendants, but i have to include it because c++ has mothingfucking trust issues -jk
		
		virtual void fixResolution();
	protected:
		Camera2D* _camera;
		glm::vec2 _position;
	};
}


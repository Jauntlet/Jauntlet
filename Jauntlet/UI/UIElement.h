#pragma once

#include <glm/glm.hpp>
#include <string>

#include "../Camera2D.h"

namespace Jauntlet {
	class UIElement
	{
	public:
		enum ELEMENT_TYPE {TEXT, SPRITE, BAR};
		UIElement();
		//constructor with _position; and camera
		UIElement(Camera2D* camera);
		//Update UI when camera is reized
		void update();
		
		void fixResolution();
	protected:
		Camera2D* _camera;
	};
}


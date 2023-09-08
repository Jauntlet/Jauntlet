#pragma once

#include <glm/glm.hpp>
#include <vector>

#include "../Camera2D.h"
#include "../Errors.h"
#include "UIElement.h"

namespace Jauntlet {
	class UIManager
	{
	public:
		//constructors;
		UIManager(Camera2D* camera);
		
		// update all ui elements to fit the resolution
		void fixResolution();
		void addElement(UIElement* uiElement);
		void removeElement(UIElement* uiElement);
		void update();
	private:
		Camera2D* _camera;
		std::vector<UIElement*> _uiElements;
	};
}


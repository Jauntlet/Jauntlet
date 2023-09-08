#pragma once

#include <glm/glm.hpp>
#include <string>

#include "../Camera2D.h"
#include "UIElement.h"

namespace Jauntlet {
	class UITextElement : UIElement
	{
	public:
		//constructor with _position;
		UITextElement(Camera2D* camera, std::string* text);
		void update();
	private:
		std::string* _text;
	};
}


#pragma once

#include <glm/glm.hpp>

#include "../Camera2D.h"

namespace Jauntlet {
	class UIElement
	{
	public:
		enum ELEMENT_TYPE {TEXT, SPRITE, BAR};
		//constructors;
		UIElement();
		//constructor with _position; and camera
		UIElement(glm::vec2 pos, Camera2D* camera, bool achored = false);
		//Update UI when camera is reized
		void update();
		//set UI position
		void setPos(glm::vec2 h);
		//anchors ui eliment to a camera
		void anchor(Camera2D* camera);

		void fixResolution(int w, int h);
	private:
		void _scale(Camera2D* camera);
		glm::vec2 _position;
		Camera2D* _currentCamera;
		bool _isAchored;
	};
}


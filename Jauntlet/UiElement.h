#pragma once

#include <glm/glm.hpp>
#include "Camera2D.h"

namespace Jauntlet {
	class UiElement
	{
	public:
		//constructors;
		UiElement();
		//constructor with _position; and camera
		UiElement(glm::vec2 pos, Camera2D* camera, bool achored = false);
		//Update UI when camera is reized
		void Update();
		//set UI position
		void SetPos(glm::vec2 h);
		//anchors ui eliment to a camera
		void Anchor(Camera2D* camera);
		//gets pointer to  camera(really only useful for debugging)
		Camera2D* GetCamera() const {return _currentCamera;}
		//get position
		glm::vec2 GetPosition() const {return _position;}
	private:
		void _Scale(Camera2D* camera);
		glm::vec2 _position;
		Camera2D* _currentCamera;
		bool _isAchored;
	};
}


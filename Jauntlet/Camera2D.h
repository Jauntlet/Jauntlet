#pragma once

#include<glm/glm.hpp>
#include<glm/gtc/matrix_transform.hpp>

namespace Jauntlet {
	class Camera2D
	{
	public:
		Camera2D();

		void init(int screenWidth, int screenHeight);
		void update();

		// setters
		void setPosition(const glm::vec2& newPosition) { 
			_position = newPosition; 
			_needsMatrixUpdate = true;
		}
		void setScale(float newScale) { 
			_scale = newScale; 
			_needsMatrixUpdate = true;
		}
		
		// getters
		glm::vec2 getPosition() { return _position; }
		glm::mat4 getCameraMatrix() { return _cameraMatrix; }
		float getScale() { return _scale; }
		

	private:
		float _scale;
		glm::vec2 _position;
		glm::mat4 _cameraMatrix;
		glm::mat4 _orthoMatrix;
		int _screenWidth, _screenHeight;
		
		bool _needsMatrixUpdate;
	};
}


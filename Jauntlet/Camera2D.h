#pragma once
#include<glm/glm.hpp>
#include<glm/gtc/matrix_transform.hpp>

namespace Jauntlet {
// Camera with a 2D perspective
class Camera2D
{
public:
	Camera2D();
	// initialize the screen size of the camera
	void init(int screenWidth, int screenHeight);
	// update the camera
	void update();
	
	// converts from a position on the cameras screen to a position in world space
	glm::vec2 convertScreenToWorld(glm::vec2 screenCoords);

	glm::vec2 convertWorldToScreen(glm::vec2 worldCoords);
	
	// update the cameras size. This is meant for if the size of the Window is updated.
	void updateCameraSize(int screenWidth, int screenHeight);
	
	// defines a new position for the camera to be
	void setPosition(const glm::vec2& newPosition) { 
		_position = newPosition; 
		_needsMatrixUpdate = true;
	}
	// sets the cameras zoom on the screen (default: 1)
	void setScale(float newScale) { 
		_scale = newScale; 
		_needsMatrixUpdate = true;
	}
	
	// get the position of the camera
	glm::vec2 getPosition() { return _position; }
	// get the cameras matrix
	glm::mat4 getCameraMatrix() { return _cameraMatrix; }
	// get the scale of the camera
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
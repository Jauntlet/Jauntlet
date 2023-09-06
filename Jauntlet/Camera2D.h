#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

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
	
	// converts from a position in world space to a position on the screen
	glm::vec2 convertWorldToScreen(glm::vec2 worldCoords);
	// converts from a position on the screen to a position in world space
	glm::vec2 convertScreenToWorld(glm::vec2 screenCoords);
	
	// update the cameras size. This is meant for if the size of the Window is updated.
	void updateCameraSize(int screenWidth, int screenHeight);

	// Test to see if bounding box is within the cameras dimensions; good for culling
	bool isBoxInView(const glm::vec2& position, const glm::vec2& dimensions);
	
	// defines a new position for the camera to be
	void setPosition(const glm::vec2& newPosition);
	// sets the cameras zoom on the screen (default: 1)
	void setScale(float newScale);
	void setScale(float newScale, bool setIntended);

	void setIntendedScale(float newscale);
	
	// get the position of the camera
	glm::vec2 getPosition() { return _position; }
	// get the cameras matrix
	glm::mat4 getCameraMatrix() { return _cameraMatrix; }
	// get the scale of the camera
	float getScale() { return _scale; }
private:
	float _scale = 1;
	float _intendedScale = 1;
	glm::vec2 _position = glm::vec2(0);
	glm::mat4 _cameraMatrix = glm::mat4(1);
	glm::mat4 _orthoMatrix = glm::mat4(1);
	int _screenWidth = 500, _screenHeight = 500;
	
	bool _needsMatrixUpdate = true;

	const float _CAMERA_MIN_ZOOM = 0.05f;
	const float _CAMERA_MAX_ZOOM = 3.50f;
};
}
#include "Camera2D.h"

using namespace Jauntlet;

Camera2D::Camera2D() : _scale(1), _position(0,0), _cameraMatrix(1), _orthoMatrix(1), _needsMatrixUpdate(true), _screenWidth(500), _screenHeight(500) {

}

void Camera2D::init(int screenWidth, int screenHeight) {
	_screenWidth = screenWidth;
	_screenHeight = screenHeight;
	_orthoMatrix = glm::ortho(0.0f, (float)_screenWidth, 0.0f, (float)_screenHeight);
}

void Camera2D::update() {
	if (_needsMatrixUpdate) {
		_orthoMatrix = glm::ortho(0.0f, (float)_screenWidth, 0.0f, (float)_screenHeight);

		glm::vec3 translate(-_position.x + _screenWidth / 2, -_position.y + _screenHeight / 2, 0.0f);
		_cameraMatrix = glm::translate(_orthoMatrix, translate);

		glm::vec3 scale(_scale, _scale, 0.0f);
		_cameraMatrix = glm::scale(_cameraMatrix, scale);

		_needsMatrixUpdate = false;
	}
}

glm::vec2 Camera2D::convertScreenToWorld(glm::vec2 screenCoords) {
	// make 0 the center of the screen and invert the Y axis
	screenCoords -= glm::vec2(_screenWidth / 2, (_screenHeight - screenCoords.y) / 2);
	// scale the coords
	screenCoords /= _scale;
	// Translate based on camera position
	screenCoords += _position;

	return screenCoords;
}
glm::vec2 Camera2D::convertWorldToScreen(glm::vec2 worldCoords) {
	worldCoords -= _position; 
	
	worldCoords *= _scale; 
	
	worldCoords += glm::vec2(_screenWidth / 2, (_screenHeight + worldCoords.y) / 2);

	return worldCoords;
}

void Camera2D::updateCameraSize(int screenWidth, int screenHeight) {
	_screenWidth = screenWidth;
	_screenHeight = screenHeight;

	_needsMatrixUpdate = true;
}
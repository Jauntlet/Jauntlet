#include <algorithm>

#include "Camera2D.h"
#include "Time.h"

using namespace Jauntlet;

Camera2D::Camera2D() {
	// Empty
}

void Camera2D::init(int screenWidth, int screenHeight) {
	_screenWidth = screenWidth;
	_screenHeight = screenHeight;
	_orthoMatrix = glm::ortho(0.0f, (float)_screenWidth, 0.0f, (float)_screenHeight);
}

void Camera2D::update() {

	if (_intendedScale != _scale) {
		_scale += (_intendedScale - _scale) * (Jauntlet::Time::getDeltaTime() * 4);
		_needsMatrixUpdate = true;

		if(std::fabs(_scale - _intendedScale) < 0.005f) {
			_scale = _intendedScale;
		}
	}

	if (!_needsMatrixUpdate) {
		return;
	}

	_orthoMatrix = glm::ortho(0.0f, (float)_screenWidth, 0.0f, (float)_screenHeight);

	glm::vec3 translate(-_position.x + _screenWidth / 2.0f, -_position.y + _screenHeight / 2.0f, 0.0f);
	_cameraMatrix = glm::translate(_orthoMatrix, translate);

	glm::vec3 scale(_scale, _scale, 0.0f);
	_cameraMatrix = glm::scale(_cameraMatrix, scale);

	_needsMatrixUpdate = false;
}

glm::vec2 Camera2D::convertWorldToScreen(glm::vec2 worldCoords) {
	// make 0 the center of the screen and invert the Y axis
	worldCoords += glm::vec2(_screenWidth / 2, _screenHeight / 2);
	// scale the coords
	worldCoords /= _scale;
	// Translate based on camera position
	worldCoords -= _position;

	return worldCoords;
}
glm::vec2 Camera2D::convertScreenToWorld(glm::vec2 screenCoords) {
	// make 0 the center of the screen and invert the Y axisglm::vec2(16,16)
	screenCoords += glm::vec2(_screenWidth / 2, _screenHeight / 2);
	// scale the coords
	screenCoords *= _scale;
	// Translate based on camera position
	screenCoords -= _position;
	
	return screenCoords;
}

void Camera2D::updateCameraSize(int screenWidth, int screenHeight) {
	_screenWidth = screenWidth;
	_screenHeight = screenHeight;

	_needsMatrixUpdate = true;
}

bool Camera2D::isBoxInView(const glm::vec2& position, const glm::vec2& dimensions) {
	const float MIN_DISTANCE_X = dimensions.x / 2 + _screenWidth / _scale / 2;
	const float MIN_DISTANCE_Y = dimensions.y / 2 + _screenHeight / _scale / 2;

	glm::vec2 centerPos = position + dimensions / 2.0f;

	glm::vec2 distVec = centerPos - _position;

	float xDepth = MIN_DISTANCE_X - std::abs(distVec.x);
	float yDepth = MIN_DISTANCE_Y - std::abs(distVec.y);

	if (xDepth > 0 && yDepth > 0) {
		return true;
	}
	return false;
}

void Camera2D::setPosition(const glm::vec2& newPosition) { 
	_position = round(newPosition); 
	_needsMatrixUpdate = true;
}

void Camera2D::setScale(float newScale) { 
	_scale = std::min(std::max(newScale,_CAMERA_MIN_ZOOM),_CAMERA_MAX_ZOOM);
	_needsMatrixUpdate = true;
}

void Camera2D::setScale(float newScale, bool setIntended) {
	_scale = std::min(std::max(newScale,_CAMERA_MIN_ZOOM),_CAMERA_MAX_ZOOM);
	_needsMatrixUpdate = true;

	if (setIntended) {
		_intendedScale = _scale;
	}
}

void Camera2D::setIntendedScale(float newScale) { 
	_intendedScale = std::min(std::max(newScale,_CAMERA_MIN_ZOOM),_CAMERA_MAX_ZOOM);
	_needsMatrixUpdate = true;
}
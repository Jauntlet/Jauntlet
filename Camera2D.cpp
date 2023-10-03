#include <algorithm>
#include <iostream>

#include "Camera2D.h"
#include "Errors.h"
#include "Time.h"
#include "glm/fwd.hpp"

using namespace Jauntlet;

Camera2D::Camera2D() {
	// Empty
}

void Camera2D::init(int screenWidth, int screenHeight, bool doRounding) {
	_screenWidth = screenWidth;
	_screenHeight = screenHeight;

	_screenVector.x = (float)screenWidth;
	_screenVector.y = (float)screenHeight;

	_orthoMatrix = glm::ortho(0.0f, (float)_screenWidth, 0.0f, (float)_screenHeight);
	_doRounding = doRounding;
}

void Camera2D::update() {
	if (_doTransitionScale && _transitionScale != _scale) {
		_scale += (_transitionScale - _scale) * (Jauntlet::Time::getDeltaTime() * 4);
		_needsMatrixUpdate = true;

		if (std::fabs(_scale - _transitionScale) < 0.005f) {
			_scale = _transitionScale;
		}
	}

	if (_doTransitionPosition && _transitionPosition != _position) {
		_position = _position + (_transitionPosition - _position) * (Jauntlet::Time::getDeltaTime() * 4);
		_needsMatrixUpdate = true;

		if (std::fabs(_position.x - _transitionPosition.x) < 0.005f && std::fabs(_position.y - _transitionPosition.y) < 0.005f) {
			_position = _transitionPosition;
		}
	}

	if (!_needsMatrixUpdate) {
		return;
	}

	_orthoMatrix = glm::ortho(0.0f, (float)_screenWidth, 0.0f, (float)_screenHeight);

	//xander wants to remove unrounded positions from the camera, as they cause shimmering. to prevent this, we round position before updating the matrix. -jk
    _cameraMatrix = glm::translate(
        _orthoMatrix,
        glm::vec3(_screenWidth / 2.0f - (_doRounding ? round(_position.x) : _position.x),
                  _screenHeight / 2.0f - (_doRounding ? round(_position.y) : _position.y), 0.0f));
    
	_cameraMatrix = glm::scale(_cameraMatrix, glm::vec3(_scale, _scale, 0));

	_needsMatrixUpdate = false;
}

void Camera2D::setActiveCamera(GLSLProgram* activeProgram) {
	glUniformMatrix4fv(activeProgram->getUniformLocation("Projection"), 1, GL_FALSE, &getCameraMatrix()[0][0]);
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
	// make 0 the center of the screen and invert the Y axis
	screenCoords -= glm::vec2(_screenWidth / 2, _screenHeight / 2);
	// Translate based on camera position
	screenCoords += glm::vec2(_position.x, -_position.y);
	// flip the y coordinates
	screenCoords.y *= -1;
	// scale the coords
	screenCoords /= _scale;
	
	return screenCoords;
}

glm::vec2 Camera2D::convertScreenToWorldDisreguardPosition(glm::vec2 screenCoords) {
	// make 0 the center of the screen and invert the Y axis
	screenCoords -= glm::vec2(_screenWidth / 2, _screenHeight / 2);
	// flip the y coordinates
	screenCoords.y *= -1;
	// scale the coords
	screenCoords /= _scale;
	
	return screenCoords;
}

void Camera2D::updateCameraSize(int screenWidth, int screenHeight) {
	_screenWidth = screenWidth;
	_screenHeight = screenHeight;

	_screenVector.x = (float)screenWidth;
	_screenVector.y = (float)screenHeight;

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
	_position = _transitionPosition = newPosition;
	_needsMatrixUpdate = true;
	_doTransitionPosition = false;
}

void Camera2D::transitionToPosition(const glm::vec2& newPosition) { 
	_transitionPosition = newPosition;
	_needsMatrixUpdate = true;
	_doTransitionPosition = true;
}

void Camera2D::setScale(float newScale) {
	_scale = _transitionScale = std::min(std::max(newScale,_CAMERA_MIN_ZOOM),_CAMERA_MAX_ZOOM);
	_needsMatrixUpdate = true;
	_doTransitionScale = false;
}

void Camera2D::transitionToScale(float newScale) { 
	_transitionScale = std::min(std::max(newScale,_CAMERA_MIN_ZOOM),_CAMERA_MAX_ZOOM);
	_needsMatrixUpdate = true;
	_doTransitionScale = true;
}

void Camera2D::translate(const glm::vec2& deltaPosition) { 
	_position += deltaPosition;
	_needsMatrixUpdate = true;
}

void Camera2D::clearTransition(TRANSITION_TYPE transitionType) {
	switch (transitionType) {
	case TRANSITION_TYPE::POSITION:
			_doTransitionPosition = false;
			break;
	case TRANSITION_TYPE::SCALE:
			_doTransitionScale = false;
			break;
		default:
			Jauntlet::fatalError("invalid TRANSITION_TYPE in clearTransition");
			break;
	}
}

void Camera2D::clearTransitions() {
	_doTransitionScale = false;
	_doTransitionPosition = false;
}

void Camera2D::multiplyPosition(float scale) {
	_position *= scale;
}

void Camera2D::multiplyScale(float scale) {
	_scale *= scale;
}

void Camera2D::multiply(float scale) {
	_scale *= scale;
	_position *= scale;
}

glm::vec2 Camera2D::getCameraSize() {
	return _screenVector;
}
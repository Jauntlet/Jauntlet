#include <algorithm>

#include "Camera2D.h"
#include "../../Errors.h"
#include "glm/fwd.hpp"

using namespace Jauntlet;

Camera2D::Camera2D(int screenWidth, int screenHeight, bool doRounding) {
	_screenWidth = screenWidth;
	_screenHeight = screenHeight;

	_orthoMatrix = glm::ortho(0.0f, (float)_screenWidth, 0.0f, (float)_screenHeight);
	_doRounding = doRounding;
}
Camera2D::Camera2D(glm::vec2 screenSize, bool doRounding) {
	_screenWidth = screenSize.x;
	_screenHeight = screenSize.y;

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
		_position += (_transitionPosition - _position) * (Jauntlet::Time::getDeltaTime() * 4);
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
        glm::vec3(_screenWidth * 0.5f - (_doRounding ? round(_position.x) : _position.x),
                  _screenHeight * 0.5f - (_doRounding ? round(_position.y) : _position.y), 0.0f));
    
	_cameraMatrix = glm::scale(_cameraMatrix, glm::vec3(_scale, _scale, 0));

	_needsMatrixUpdate = false;
}

void Camera2D::setActive() {
	if (GLSLProgram::currentProgram != nullptr) {
#ifdef OPENGL
		glUniformMatrix4fv(GLSLProgram::currentProgram->getUniformLocation("Projection"), 1, GL_FALSE, &getMatrix()[0][0]);
#endif
	}
}

glm::vec2 Camera2D::convertWorldToScreen(glm::vec2 worldCoords) const {
	// make 0 the center of the screen and invert the Y axis
	worldCoords += glm::vec2(_screenWidth * 0.5f, _screenHeight * 0.5f);
	// scale the coords
	worldCoords /= _scale;
	// Translate based on camera position
	worldCoords -= _position;

	return worldCoords;
}
glm::vec2 Camera2D::convertScreenToWorld(glm::vec2 screenCoords) const {
	// make 0 the center of the screen and invert the Y axis
	screenCoords -= glm::vec2(_screenWidth * 0.5f, _screenHeight * 0.5f);
	// Translate based on camera position
	screenCoords += glm::vec2(_position.x, -_position.y);
	// flip the y coordinates
	screenCoords.y *= -1;
	// scale the coords
	screenCoords /= _scale;
	
	return screenCoords;
}

void Camera2D::updateSize(int screenWidth, int screenHeight) {
	_screenWidth = screenWidth;
	_screenHeight = screenHeight;

	_needsMatrixUpdate = true;
}
void Camera2D::updateSize(glm::ivec2 screenSize) {
	_screenWidth = screenSize.x;
	_screenHeight = screenSize.y;

	_needsMatrixUpdate = true;
}

bool Camera2D::isBoxInView(const glm::vec4& destinationRect) const {
	const float MIN_DISTANCE_X = destinationRect.z * 0.5f + _screenWidth * _scale * 0.5f;
	const float MIN_DISTANCE_Y = destinationRect.w * 0.5f + _screenHeight * _scale * 0.5f;

	glm::vec2 centerPos = glm::vec2(destinationRect.x, destinationRect.y) + glm::vec2(destinationRect.z, destinationRect.w) * 0.5f;

	glm::vec2 distVec = centerPos - _position;

	float xDepth = MIN_DISTANCE_X - std::abs(distVec.x);
	float yDepth = MIN_DISTANCE_Y - std::abs(distVec.y);

	if (xDepth > 0 && yDepth > 0) {
		return true;
	}
	return false;
}
bool Camera2D::isBoxInView(const glm::vec2& position, const glm::vec2& dimensions) const {
	const float MIN_DISTANCE_X = dimensions.x * 0.5f + _screenWidth * _scale * 0.5f;
	const float MIN_DISTANCE_Y = dimensions.y * 0.5f + _screenHeight * _scale * 0.5f;

	glm::vec2 centerPos = position + dimensions * 0.5f;

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
	if (_doTransitionScale) {
		_transitionPosition = newPosition;
	} else {
		_transitionPosition = newPosition * _transitionScale;
	}
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
	_transitionPosition *= _transitionScale;
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
			Jauntlet::error("invalid TRANSITION_TYPE in clearTransition");
			break;
	}
}

void Camera2D::clearTransitions() {
	_doTransitionScale = false;
	_doTransitionPosition = false;
}

void Camera2D::multiplyPosition(float scale) {
	_position *= scale;
	_needsMatrixUpdate = true;
}

void Camera2D::multiplyScale(float scale) {
	_scale *= scale;
	_needsMatrixUpdate = true;
}

void Camera2D::multiply(float scale) {
	_scale *= scale;
	_position *= scale;
	_needsMatrixUpdate = true;
}

glm::vec2 Camera2D::getSize() {
	return glm::vec2(_screenWidth, _screenHeight);
}
glm::vec2 Camera2D::getPosition() {
	return _position;
}
float Camera2D::getScale() {
	return _scale;
}
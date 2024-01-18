#include "Camera.h"
#include "../GLSLProgram.h"
#include <glm/ext/matrix_clip_space.hpp>
#include<glm/gtx/rotate_vector.hpp>
#include<glm/gtx/vector_angle.hpp>

Camera::Camera(int screenWidth, int screenHeight) 
	:
	_screenSize(screenWidth, screenHeight)
{
	// Empty
}
Camera::Camera(glm::ivec2 screenSize) 
	:
	_screenSize(screenSize)
{
	// Empty
}

void Camera::update() {
	if (!_needsMatrixUpdate) {
		return;
	}

	_projectionMatrix = glm::perspective(glm::radians(_fov), (float)_screenSize.x / (float)_screenSize.y, 0.1f, 100.0f);

	_viewMatrix = glm::lookAt(_position, _position + _orientation, _up);

	_mvp = _projectionMatrix *_viewMatrix;

	_needsMatrixUpdate = false;
}

void Camera::setActive() {
	if (GLSLProgram::currentProgram != nullptr) {
		glUniformMatrix4fv(GLSLProgram::currentProgram->getUniformLocation("Projection"), 1, GL_FALSE, &_mvp[0][0]);
	}
}

void Camera::setPosition(const glm::vec3& newPosition) {
	_position = newPosition;
	_needsMatrixUpdate = true;
}

void Camera::translate(const glm::vec3& addedPosition) {
	_position += addedPosition;
	_needsMatrixUpdate = true;
}

void Camera::rotateX(float radians){
	_orientation = glm::rotate(_orientation, -radians, _up);
	_needsMatrixUpdate = true;
}
void Camera::rotateY(float radians) {
	glm::vec3 newOrientation = glm::rotate(_orientation, -radians, glm::normalize(glm::cross(_orientation, _up)));

	// prevents camera from flipping.
	if (abs(glm::angle(newOrientation, _up) - glm::radians(90.0f)) <= glm::radians(85.0f)) {
		_orientation = newOrientation;
	}
	_needsMatrixUpdate = true;
}

glm::vec3 Camera::getPosition() {
	return _position;
}
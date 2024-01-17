#pragma once
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>
#include <glm/ext/matrix_transform.hpp>

class Camera
{
public:
	Camera(int screenWidth, int screenHeight);
	Camera(glm::ivec2 screenSize);

	// update the camera
	void update();
	// Makes this camera the camera objects are rendered to.
	void setActive();

	// defines a new position for the camera to be.
	void setPosition(const glm::vec3& newPosition);
	// add to the current camera position
	void translate(const glm::vec3& addedPosition);

	void rotateX(float radians);
	void rotateY(float radians);

	glm::vec3 getPosition();

private:
	glm::ivec2 _screenSize;
	
	glm::vec3 _position = glm::vec3(0.0f, 0.0f, 3.0f);
	glm::vec3 _up = glm::vec3(0.0f, 1.0f, 0.0f);
	glm::vec3 _orientation = glm::vec3(0.0f, 0.0f, -1.0f);

	float _yaw = -90.0f, _pitch = 0.0f, _fov = 45.0f;

	bool _needsMatrixUpdate = true;
	glm::mat4 _projectionMatrix, _viewMatrix;
	glm::mat4 _mvp;
};


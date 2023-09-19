#pragma once
#include "Rendering/GLSLProgram.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace Jauntlet {
// Camera with a 2D perspective
class Camera2D
{
public:
	enum class TRANSITION_TYPE {POSITION, SCALE};

	Camera2D();
	// initialize the screen size of the camera
	void init(int screenWidth, int screenHeight);
	// update the camera
	void update();
	// Makes this camera the camera objects are rendered to.
	void setActiveCamera(GLSLProgram* activeProgram);
	
	// converts from a position in world space to a position on the screen
	glm::vec2 convertWorldToScreen(glm::vec2 worldCoords);
	// converts from a position on the screen to a position in world space
	glm::vec2 convertScreenToWorld(glm::vec2 screenCoords);
	// converts from a position in world space to a position on the screen
	glm::vec2 convertScreenToWorldDisreguardPosition(glm::vec2 screenCoords);
	
	// update the cameras size. This is meant for if the size of the Window is updated.
	void updateCameraSize(int screenWidth, int screenHeight);

	// Test to see if bounding box is within the cameras dimensions; good for culling
	bool isBoxInView(const glm::vec2& position, const glm::vec2& dimensions);
	
	// defines a new position for the camera to be.
	void setPosition(const glm::vec2& newPosition);
	// makes the camera transition to this position smoothly over time.
	void transitionToPosition(const glm::vec2& newPosition);

	// sets the cameras zoom on the screen (default: 1).
	void setScale(float newScale);
	/// makes the camera transition to this scale smoothly over time.
	void transitionToScale(float newscale);

	// translates the camera
	void translate(const glm::vec2& deltaPosition);

	// clears a transition, takes enum TRANSITION_TYPE
	void clearTransition(TRANSITION_TYPE transitionType);
	// clears all transitions.
	void clearTransitions();
	
	// get the cameras matrix
	glm::mat4 getCameraMatrix() { return _cameraMatrix; }
	
	// multiply the position of the camera by a float
	void multiplyPosition(float scale);
	// multiply the scale of the camera by a float
	void multiplyScale(float scale);
	// multiply both the position and scale of the camera by a float
	void multiply(float scale);
private:
	float _scale = 1;
	float _transitionScale = 1;

	glm::vec2 _position = glm::vec2(0);
	glm::vec2 _transitionPosition = glm::vec2(0);

	glm::mat4 _cameraMatrix = glm::mat4(1);
	glm::mat4 _orthoMatrix = glm::mat4(1);

	int _screenWidth = 500, _screenHeight = 500;
	
	bool _needsMatrixUpdate = true;

	bool _doTransitionPosition = false;
	bool _doTransitionScale = false;

	float _CAMERA_MIN_ZOOM = 0.05f;
	float _CAMERA_MAX_ZOOM = 3.50f;
};
}
/* Purpose: Optimal camera for 2D games, which handles the state of said perspective and all data related to it.
 * Main Contributer(s): Xander Mooney / Jack Kennedy
 */

#pragma once

#include <glm/ext/matrix_transform.hpp>
#include <glm/ext/matrix_clip_space.hpp>
#include <glm/mat4x4.hpp>
#include "../GLSLProgram.h"
#include "../../Time.h"

namespace Jauntlet {
// Camera with a 2D perspective
class Camera2D
{
public:
	enum class TRANSITION_TYPE {POSITION, SCALE};

	Camera2D(int screenWidth, int screenHeight, bool doRounding = true);
	Camera2D(glm::vec2 screenSize, bool doRounding = true);
	// update the camera
	void update();
	// Makes this camera the camera objects are rendered to.
	void setActive();
	
	// converts from a position in world space to a position on the screen
	glm::vec2 convertWorldToScreen(glm::vec2 worldCoords) const;
	// converts from a position on the screen to a position in world space
	glm::vec2 convertScreenToWorld(glm::vec2 screenCoords) const;
	
	// update the cameras size. This is meant for if the size of the Window is updated.
	void updateSize(int screenWidth, int screenHeight);
	// update the cameras size. This is meant for if the size of the Window is updated.
	void updateSize(glm::ivec2 screenSize);

	// Test to see if bounding box is within the cameras dimensions; good for culling
	bool isBoxInView(const glm::vec4& destinationRect) const;
	// Test to see if bounding box is within the cameras dimensions; good for culling
	bool isBoxInView(const glm::vec2& position, const glm::vec2& dimensions) const;
	
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
	glm::mat4 getMatrix() { return _cameraMatrix; }
	
	// multiply the position of the camera by a float
	void multiplyPosition(float scale);
	// multiply the scale of the camera by a float
	void multiplyScale(float scale);
	// multiply both the position and scale of the camera by a float
	void multiply(float scale);

	// get the cameras width and height as a vec2 for maths
	glm::vec2 getSize(); // UNDOCUMENTED
	glm::vec2 getPosition(); // UNDOCUMENTED

private:
	float _scale = 1;
	float _transitionScale = 1;

	glm::vec2 _position = glm::vec2(0);
	glm::vec2 _transitionPosition = glm::vec2(0);

	glm::mat4 _cameraMatrix = glm::mat4(1);
	glm::mat4 _orthoMatrix = glm::mat4(1);

	int _screenWidth = 500, _screenHeight = 500;

	glm::vec2 _screenVector = glm::vec2(1,1);
	
	bool _needsMatrixUpdate = true;

	bool _doTransitionPosition = false;
	bool _doTransitionScale = false;

	float _CAMERA_MIN_ZOOM = 0.05f;
	float _CAMERA_MAX_ZOOM = 3.50f;

	bool _doRounding = true;
};
}
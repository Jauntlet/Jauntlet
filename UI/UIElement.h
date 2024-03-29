/* Purpose: Creates a basic, blank, and barely functional UI element.
 * better used as a base for other elements.
 * Main Contributer(s): Jack Kennedy
 */

#pragma once

#include <glm/ext/vector_float2.hpp>
#include <string>

#include "../Rendering/Cameras/Camera2D.h"
#include "glm/fwd.hpp"
#include "../Rendering/Textures/SpriteBatch.h"

class UIElement
{
public:
	enum class ORIGIN_PIN {
		TOP_LEFT = 0,
		TOP = 1,
		TOP_RIGHT = 2,
		RIGHT = 3,
		BOTTOM_RIGHT = 4,
		BOTTOM = 5,
		BOTTOM_LEFT = 6,
		LEFT = 7,
		CENTER = 8
	};

	UIElement();

	virtual void draw(Camera2D* camera, SpriteBatch* spriteBatch, float scale);
	virtual void resolvePosition(Camera2D* camera, glm::vec2* resolvedPins, float scale);
	
	bool visible = true;
protected:
	//Camera2D* _camera;
	glm::vec2* _position;
	glm::vec2 _resolvedPosition;
	glm::vec2 _resolvedOrigin;
	ORIGIN_PIN _originPin = ORIGIN_PIN::TOP_LEFT;
};
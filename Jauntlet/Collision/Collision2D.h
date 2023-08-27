#pragma once

#include "Collider2D.h"

#include <vector>
#include <glm/glm.hpp>

namespace Jauntlet
{
	class Collision2D //the returned class for collision data
	{
	public:
		//constructor (requires the two colliding objects)
		Collision2D(Collider2D parent, Collider2D other);

		//not functional yet
		//std::vector<glm::vec2> GetCollisionPoints();

		//returns a vector2  regarding a direction (0,0 would be dead center) with a magnitude of 1.
		glm::vec2 GetNormal();
	private:
		//still not functional
		//std::vector<std::vector<float>> _collisionPoints; 
		//normal at the point of collision
		glm::vec2 _normal;
		//Main collider in calculation
		Collider2D _parent;
		//the collider that collided with during the check
		Collider2D _other;
	};
}
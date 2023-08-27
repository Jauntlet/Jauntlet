#include "Collision2D.h"

#include <cmath>
#include <glm/glm.hpp>

using namespace Jauntlet;

Collision2D::Collision2D(Collider2D parent, Collider2D other) : _parent(parent), _other(other) //constructor
{
	//_parent = parent; //the collider that did the check
	//_other = other; //the collided-with collider

	//calculate normal
	float _x = other.position.x - parent.position.x; //gets a relative distance between the colliders as a position
	float _y = other.position.y - parent.position.y; //same for y

	//get angle of the two values
	float _angle = std::atan(_y / _x);

	//set _x and _y to values where the magnitude is 1.
	_x = std::cos(_angle);
	_y = std::sin(_angle);

	//set normal
	_normal = glm::vec2(_x, _y);
}

//std::vector<std::vector<float>> Collision2D::GetCollisionPoints() { //NOT USED / CALCULATED YET
//	return _collisionPoints;
//}

//returns a vector2 regarding a direction (0,0 would be dead center) with a magnitude of 1.
glm::vec2 Collision2D::GetNormal() {
	return _normal;
}
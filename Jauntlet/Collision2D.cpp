#include "Collision2D.h"

#include <cmath>

using namespace Jauntlet;

Collision2D::Collision2D(Collider2D parent, Collider2D other) : _parent(parent), _other(other) //constructor
{
	//_parent = parent; //the collider that did the check
	//_other = other; //the collided-with collider

	//calculate normal
	float _x = other.position[0] - parent.position[0]; //gets a relative distance between the colliders as a position
	float _y = other.position[1] - parent.position[1]; //same for y

	//get angle of the two values
	float _angle = std::atan(_y / _x);

	//set _x and _y to values where the magnitude is 1.
	_x = std::cos(_angle);
	_y = std::sin(_angle);

	//set normal
	_normal = { _x, _y };
}

std::vector<std::vector<float>> Collision2D::GetCollisionPoints() { //NOT USED / CALCULATED YET
	return _collisionPoints;
}

std::vector<float> Collision2D::GetNormal() {
	return _normal;
}

CircleCollider2D::CircleCollider2D(float radius, float offsetX, float offsetY) : _radius(radius), _offsetX(offsetX), _offsetY(offsetY) {
	//_radius = radius;
	//_offsetX = offsetX;
	//_offsetY = offsetY;

	position = { _offsetX, _offsetY };
}

void CircleCollider2D::SetRadius(float value) { //allows for the changing of size for the circle collider
	_radius = value;
}
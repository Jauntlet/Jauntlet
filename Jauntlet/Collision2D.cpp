#include "Collision2D.h"

#include <cmath>

using namespace Jauntlet;

Collision2D::Collision2D(Collider2D parent, Collider2D other)
{
	_parent = parent;
	_other = other;

	//TODO: get the position of the collider parent and the position of the collider "other" and determine the other object's position RELATIVE to the parent (pretty sure its other.pos (x or y) minus parent.pos (also x or y)) and then calculate for magnitude = 1. 
	//calculate normal
	float _x = other.position[0] - parent.position[0]; //test value, will be other.x - parent.x
	float _y = other.position[1] - parent.position[1]; //test value, will be other.y - parent.y

	//get angle of the two values
	unsigned float _angle = std::atan(_y / _x);

	//set _x and _y to values where the magnitude is 1.
	_x = std::cos(_angle);
	_y = std::sin(_angle);

	//set normal
	_normal = { _x, _y };
}

std::vector<std::array<float, 2>> Collision2D::GetCollisionPoints()
{
	return _collisionPoints;
}

std::array<float, 2> Collision2D::GetNormal()
{
	return _normal;
}

CircleCollider2D::CircleCollider2D(float radius, float offsetX = 0, float offsetY = 0)
{
	_radius = radius;
	_offsetX = offsetX;
	_offsetY = offsetY;

	position = { _offsetX, _offsetY };
}

void CircleCollider2D::SetRadius(float value)
{
	_radius = value;
}
#include "Collider2D.h"

//constructor; requires at least a radius and position x & y.
CircleCollider2D::CircleCollider2D(float radius, float x, float y, float offsetX, float offsetY) : _radius(radius) {
	//_radius = radius;
	//_offsetX = offsetX;
	//_offsetY = offsetY;
	position = glm::vec2(x, y);
	_offset = glm::vec2(offsetX, offsetY);
}

//allows for the changing of size for the circle collider
void CircleCollider2D::SetRadius(float value) {
	_radius = value;
}
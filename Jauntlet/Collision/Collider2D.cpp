#include "Collider2D.h"

//constructor; requires a radius and position (x & y).
CircleCollider2D::CircleCollider2D(float radius, float x, float y, glm::vec2 offset) : _radius(radius) {
	//_radius = radius;
	//_offsetX = offsetX;
	//_offsetY = offsetY;
	position = glm::vec2(x, y);
	_offset = offset;
}

//constructor; requires a radius and position (vec2).
CircleCollider2D::CircleCollider2D(float radius, glm::vec2 pos, glm::vec2 offset) : _radius(radius), _offset(offset) {
	position = pos;
}

//allows for the changing of size for the circle collider
void CircleCollider2D::SetRadius(float value) {
	_radius = value;
}

float CircleCollider2D::GetRadius() {
	return _radius;
}

BoxCollider2D::BoxCollider2D(float width, float height, float x, float y, float offsetX , float offsetY) {
	position = glm::vec2(x, y);
	_size = glm::vec2(width, height);
	_offset = glm::vec2(offsetX, offsetY);
}

BoxCollider2D::BoxCollider2D(glm::vec2 size, glm::vec2 pos, glm::vec2 offset) : _size(size), _offset(offset) {
	position = pos;
}

float BoxCollider2D::GetWidth() {
	return _size.x;
}

float BoxCollider2D::GetHeight() {
	return _size.y;
}

glm::vec2 BoxCollider2D::GetSize() {
	return _size;
}

void BoxCollider2D::SetWidth(float width) {
	_size = glm::vec2(width, _size.y);
}

void BoxCollider2D::SetHeight(float height) {
	_size = glm::vec2(_size.x, height);
}

void BoxCollider2D::SetSize(float width, float height) {
	_size = glm::vec2(width, height);
}

void BoxCollider2D::SetSize(glm::vec2 size) {
	_size = size;
}
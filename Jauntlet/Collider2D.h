#pragma once

#include <vector>
#include <glm/glm.hpp>

//Base class for all colliders
class Collider2D {
public:
	glm::vec2 position;
protected:
	//Sprite _linkedSprite;
};

//Circle Collider, has a radius and position x & y. Collision determined by distance from center.
class CircleCollider2D : Collider2D
{
public:
	//constructor; requires at least a radius and position x & y.
	CircleCollider2D(float radius, float x, float y, float offsetX = 0, float offsetY = 0);
	//allows the changing of the radius
	void SetRadius(float value);

private:
	float _radius; //determines how far out to check for collision
	glm::vec2 _offset; //Offset from parent object (if we parent it to something) for now determines position
};
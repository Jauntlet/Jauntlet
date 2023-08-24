#pragma once

#include <vector>
#include <glm/glm.hpp>

namespace Jauntlet
{
	//Base class for all colliders
class Collider2D {
public:
	//an array of 2 floats. [0] = X [1] = Y.
	glm::vec2 position; 
protected:
	//Sprite _linkedSprite;
};

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
}
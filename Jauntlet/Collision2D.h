#pragma once

#include <vector>

namespace Jauntlet
{
	//Base class for all colliders
class Collider2D {
public:
	//an array of 2 floats. [0] = X [1] = Y.
	std::vector<float> position; 
protected:
	//Sprite _linkedSprite;
};

class Collision2D //the returned class for collision data
{
public:
	//constructor (requires the two colliding objects)
	Collision2D(Collider2D parent, Collider2D other); 
 //not functional yet
	std::vector<std::vector<float>> GetCollisionPoints();
	// returns a "vector 2" regarding a direction (0,0 would be dead center) with a magnitude of 1.
	std::vector<float> GetNormal(); 
private:
	//still not functional
	std::vector<std::vector<float>> _collisionPoints; 
	//normal at the point of collision
	std::vector<float> _normal;
	//Main collider in calculation
	Collider2D _parent; 
	//the collider that collided with during the check
	Collider2D _other;
};
//Circle Collider, has a radius and otherwise is fairly simple. Collision determined by distance from center
class CircleCollider2D : Collider2D 
{
public:
	//constructor; only radius is required but it will spawn at 0,0.
	CircleCollider2D(float radius, float offsetX = 0, float offsetY = 0);
	//allows the changing of the radius
	void SetRadius(float value);

private:
	float _radius; //determines how far out to check for collision
	float _offsetX; //Offset from parent object (if we parent it to something) for now determines position
	float _offsetY; //See above; works for Y values instead
};
}
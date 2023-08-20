#pragma once

#include <vector>

namespace Jauntlet
{
	class Collision2D //the returned class for collision data
	{
	public:
		Collision2D(Collider2D parent, Collider2D other); //constructor (requires the two colliding objects)
		std::vector<std::array<float, 2>> GetCollisionPoints(); //not functional
		std::array<float, 2> GetNormal(); //basically returns a "vector 2" regarding a direction (0,0 would be dead center, like they're on top of eachother) with a magnitude of 1.
		//for the function above, if it returns 1,0 its facing straight to the right, 0,1 straight up, etc. etc., with values in between like at a 45 degree angle its somewhere around 0.7,0.7 but obviously the magnitude is exactly 1.
	private:
		std::vector<std::array<float, 2>> _collisionPoints; //still not functional; may also use a vector2 for positions in the future.
		float _normal[2]; //stores the calculated normal at the point of collision
		Collider2D _parent; //the collider doing the check
		Collider2D _other; //the collider collided with during the check
	};

	class Collider2D //Base class for all colliders
	{
	public:
		float position[2]; //could be a vector2 class, for now its an array of 2 floats. [0] = X [1] = Y.
	protected:
		//Sprite _linkedSprite;
	};

	class CircleCollider2D : Collider2D //Circle Collider, has a radius and otherwise is fairly simple. Collision determined by distance from center (hence the radius)
	{
	public:
		CircleCollider2D(float radius, float offsetX = 0, float offsetY = 0); //constructor; only radius is required but it will spawn at 0,0.
		void SetRadius(float value); //allows the changing of the radius

	private:
		float _radius; //determines how far out to check for collision
		float _offsetX; //Offset from parent object (if we parent it to something) for now determines position
		float _offsetY; //See above; works for Y values instead
	};
}
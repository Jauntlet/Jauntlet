#pragma once

#include <vector>

namespace Jauntlet
{
	class Collision2D
	{
	public:
		Collision2D(Collider2D parent, Collider2D other);
		std::vector<std::array<float, 2>> GetCollisionPoints();
		std::array<float, 2> GetNormal();

	private:
		std::vector<std::array<float, 2>> _collisionPoints;
		float _normal[2];
		Collider2D _parent;
		Collider2D _other;
	};

	class Collider2D
	{
	public:
		float position[2];
	protected:
		//Sprite _linkedSprite;
	};

	class CircleCollider2D : Collider2D
	{
	public:
		CircleCollider2D(float radius, float offsetX = 0, float offsetY = 0);
		void SetRadius(float value);

	private:
		float _radius;
		float _offsetX;
		float _offsetY;
	};
}
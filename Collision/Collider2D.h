#pragma once

#include <glm/glm.hpp>
#include <vector>

namespace Jauntlet
{
	//Base class for all colliders
	class Collider2D {
	public:
		glm::vec2 position;
	};

	//Circle Collider, has a radius and position x & y. Collision determined by distance from center.
	class CircleCollider2D : public Collider2D
	{
	public:
		//constructor; requires at least a radius and position x & y.
		CircleCollider2D(float radius, float x, float y, glm::vec2 offset = glm::vec2(0, 0));
		CircleCollider2D(float radius, glm::vec2 pos, glm::vec2 offset = glm::vec2(0, 0));
		
		float GetRadius();

		//allows the changing of the radius
		void SetRadius(float value);

	private:
		float _radius; //determines how far out to check for collision
		glm::vec2 _offset; //Offset from parent object (if we parent it to something) for now determines position
	};

	class BoxCollider2D : public Collider2D
	{
	public:
		BoxCollider2D(float width, float height, float x, float y, float offsetX = 0, float offsetY = 0);
		BoxCollider2D(glm::vec2 size, glm::vec2 pos, glm::vec2 offset = glm::vec2(0, 0));

		float GetWidth();
		float GetHeight();
		glm::vec2 GetSize();

		void SetWidth(float width);
		void SetHeight(float height);
		void SetSize(float width, float height);
		void SetSize(glm::vec2 size);

	private:
		glm::vec2 _size;
		glm::vec2 _offset;
	};
}
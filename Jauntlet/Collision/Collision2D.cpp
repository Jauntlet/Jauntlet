#include "Collision2D.h"

#include <cmath>
#include <glm/glm.hpp>

using namespace Jauntlet;

Collision2D::Collision2D() : _parent(nullptr), _other(nullptr)
{
	//not much here
	_normal = glm::vec2();
}

Collision2D::Collision2D(Collider2D* parent, Collider2D* other) : _parent(parent), _other(other) //constructor
{
	//_parent = parent; //the collider that did the check
	//_other = other; //the collided-with collider

	//calculate normal
	float _x = other->position.x - parent->position.x; //gets a relative distance between the colliders as a position
	float _y = other->position.y - parent->position.y; //same for y

	//get angle of the two values
	float _angle = std::atan(_y / _x);

	//set _x and _y to values where the magnitude is 1.
	_x = std::cos(_angle);
	_y = std::sin(_angle);

	//set normal
	_normal = glm::vec2(_x, _y);
}

//std::vector<std::vector<float>> Collision2D::GetCollisionPoints() { //NOT USED / CALCULATED YET
//	return _collisionPoints;
//}

//returns a vector2 regarding a direction (0,0 would be dead center) with a magnitude of 1.
glm::vec2 Collision2D::GetNormal() {
	return _normal;
}

//Circle on Circle collision check
bool Collision2D::getCollision(CircleCollider2D* parent, CircleCollider2D* other) {
	float dist = glm::sqrt(glm::pow(other->position.x - parent->position.x, 2)
			   + glm::pow(other->position.y - parent->position.y, 2));
	float totlR = parent->GetRadius() + other->GetRadius();
	
	if (dist <= totlR) {
		//set parent & other
		_parent = parent;
		_other = other;
		
		//maybe normal?
		_normal = glm::vec2(); //temporary

		//return the collision happened
		return true;
	}

	return false; //no collision
}

//Circle on Box collision check
bool Collision2D::getCollision(CircleCollider2D* parent, BoxCollider2D* other) {
	glm::vec2 relCenter = parent->position - other->position;
	glm::vec2 cornerOffset = glm::abs(relCenter) - glm::vec2(other->GetWidth() / 2, other->GetHeight() / 2);
	
	if (glm::min(glm::max(cornerOffset.x, cornerOffset.y), 0.0f) + glm::length(glm::max(cornerOffset.x, cornerOffset.y)) - parent->GetRadius() <= 0)
	{
		//set parent & other
		_parent = parent;
		_other = other;

		//initialize normal for now
		_normal = glm::vec2();

		//return the collision happened
		return true;
	}

	return false;
}

//Box on Circle collision check
bool Collision2D::getCollision(BoxCollider2D* parent, CircleCollider2D* other) {
	glm::vec2 relCenter = other->position - parent->position;
	glm::vec2 cornerOffset = glm::abs(relCenter) - glm::vec2(parent->GetWidth() / 2, parent->GetHeight() / 2);

	if (glm::min(glm::max(cornerOffset.x, cornerOffset.y), 0.0f) + glm::length(glm::max(cornerOffset.x, cornerOffset.y)) - other->GetRadius() <= 0)
	{
		//set parent & other
		_parent = parent;
		_other = other;

		//initialize normal for now
		_normal = glm::vec2();

		//return the collision happened
		return true;
	}
	
	return false;
}

bool Collision2D::getCollision(BoxCollider2D* parent, BoxCollider2D* other) {
	if (parent->position.x < other->position.x + other->GetWidth() &&
		 parent->position.x + parent->GetWidth() > other->position.x &&
		 parent->position.y < other->position.y + other->GetHeight() &&
		 parent->position.y + parent->GetHeight() > other->position.y)
	{
		//set parent & other
		_parent = parent;
		_other = other;

		//init normal
		_normal = glm::vec2();

		//return the collision happened
		return true;
	}

	return false;
}
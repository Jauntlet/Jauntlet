#include <cmath>
#include <glm/glm.hpp>

#include "../JMath.h"
#include "Collision2D.h"

using namespace Jauntlet;

Collision2D::Collision2D() {
	// Empty
}

//std::vector<std::vector<float>> Collision2D::GetCollisionPoints() { //NOT USED / CALCULATED YET
//	return _collisionPoints;
//}

//returns a vector2 regarding a direction (0,0 would be dead center) with a magnitude of 1.
glm::vec2 Collision2D::GetNormal() const {
	return _normal;
}

float Collision2D::GetOverlap() const {
	return _overlap;
}

//Circle on Circle collision check
bool Collision2D::calcCollision(CircleCollider2D* parent, CircleCollider2D* other) {
	float dist = JMath::Distance(other->position, parent->position);;

	float totlR = parent->GetRadius() + other->GetRadius();
	
	if (dist <= totlR) {
		//set parent & other
		_parent = parent;
		_other = other;
		
		//find and set normal
		_normal = glm::normalize(other->position - parent->position);

		//set overlap
		_overlap = dist - totlR;

		//return the collision happened
		return true;
	}

	return false; //no collision
}

//Circle on Box collision check
bool Collision2D::calcCollision(CircleCollider2D* parent, BoxCollider2D* other) {
	glm::vec2 relCenter = parent->position - other->position;
	//glm::vec2 cornerOffset = glm::abs(relCenter) - glm::vec2(other->GetWidth() / 2, other->GetHeight() / 2);
	//float testValue = glm::min(glm::max(cornerOffset.x, cornerOffset.y), 0.0f) + glm::length(glm::max(cornerOffset.x, cornerOffset.y)) - parent->GetRadius();

	glm::vec2 t = parent->position;

	if (t.x < other->position.x - other->GetWidth()/2) t.x = other->position.x - other->GetWidth()/2;
	else if (t.x > other->position.x + other->GetWidth()/2) t.x = other->position.x + other->GetWidth()/2;
	if (t.y < other->position.y - other->GetHeight()/2) t.y = other->position.y - other->GetHeight()/2;
	else if (t.y > other->position.y + other->GetHeight()/2) t.y = other->position.y + other->GetHeight()/2;

	float tDist = JMath::Distance(parent->position, t);

	if (tDist <= parent->GetRadius())
	{
		//set parent & other
		_parent = parent;
		_other = other;

		//find and set normal
		glm::vec2 newNormal;

		if (relCenter.x == relCenter.y) newNormal = glm::normalize(relCenter);
		else if (glm::abs(relCenter.x) > glm::abs(relCenter.y)) newNormal = glm::normalize(glm::vec2(relCenter.x, 0));
		else newNormal = glm::normalize(glm::vec2(0, relCenter.y));

		_normal = newNormal;

		//set overlap
		_overlap = glm::abs(tDist - parent->GetRadius());

		//return the collision happened
		return true;
	}

	return false;
}

//Box on Circle collision check
bool Collision2D::calcCollision(BoxCollider2D* parent, CircleCollider2D* other) {
	glm::vec2 relCenter = other->position - parent->position;
	//glm::vec2 cornerOffset = glm::abs(relCenter) - glm::vec2(parent->GetWidth() / 2, parent->GetHeight() / 2);
	//float testValue = glm::min(glm::max(cornerOffset.x, cornerOffset.y), 0.0f) + glm::length(glm::max(cornerOffset.x, cornerOffset.y)) - other->GetRadius();
	
	glm::vec2 t = other->position;

	if (t.x < parent->position.x - parent->GetWidth() / 2) t.x = parent->position.x - parent->GetWidth() / 2;
	else if (t.x > parent->position.x + parent->GetWidth() / 2) t.x = parent->position.x + parent->GetWidth() / 2;
	if (t.y < parent->position.y - parent->GetHeight() / 2) t.y = parent->position.y - parent->GetHeight() / 2;
	else if (t.y > parent->position.y + parent->GetHeight() / 2) t.y = parent->position.y + parent->GetHeight() / 2;

	float tDist = JMath::Distance(other->position, t);

	if (tDist <= other->GetRadius())
	{
		//set parent & other
		_parent = parent;
		_other = other;

		//initialize normal for now
		_normal = glm::normalize(other->position - parent->position);

		//set overlap
		_overlap = glm::abs(tDist - other->GetRadius());

		//return the collision happened
		return true;
	}
	
	return false;
}

//Box on Box collision check
bool Collision2D::calcCollision(BoxCollider2D* parent, BoxCollider2D* other) {
	if (parent->position.x < other->position.x + other->GetWidth() &&
		 parent->position.x + parent->GetWidth() > other->position.x &&
		 parent->position.y < other->position.y + other->GetHeight() &&
		 parent->position.y + parent->GetHeight() > other->position.y)
	{
		//set parent & other
		_parent = parent;
		_other = other;

		//find and set normal
		glm::vec2 newNormal;

		glm::vec2 relCenter = other->position - parent->position;

		if (relCenter.x == relCenter.y) newNormal = glm::normalize(relCenter);
		else if (glm::abs(relCenter.x) > glm::abs(relCenter.y)) newNormal = relCenter.x > 0 ? glm::vec2(-1, 0) : glm::vec2(1, 0);
		else newNormal = relCenter.y > 0 ? glm::vec2(0, -1) : glm::vec2(0, 1);

		_normal = newNormal;

		//set overlap
		_overlap = glm::sqrt(glm::pow(other->position.x - parent->position.x, 2) //a^2 + b^2 = c^2 a/b being width/height and c being the line to the circle hypothetically
			+ glm::pow(other->position.y - parent->position.y, 2)) - (glm::sqrt(glm::pow(parent->GetWidth(), 2) + glm::pow(parent->GetHeight(), 2)) + glm::sqrt(glm::pow(other->GetWidth(), 2) + glm::pow(other->GetHeight(), 2)));

		//return the collision happened
		return true;
	}

	return false;
}

//Circle with given position collision check
bool Collision2D::calcCollision(CircleCollider2D* parent, glm::vec2 other) {
	if (JMath::Distance(parent->position + glm::vec2(parent->GetRadius()), other) <= parent->GetRadius()) return true;
	return false;
}

//Box with given position collision check
bool Collision2D::calcCollision(BoxCollider2D* parent, glm::vec2 other) {
	if (other.x >= parent->position.x &&
		 other.x <= parent->position.x + parent->GetWidth() &&
		 other.y >= parent->position.y &&
		 other.y <= parent->position.y + parent->GetHeight()) return true;
	return false;
}
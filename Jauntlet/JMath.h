#pragma once
// JMath is purposefully left out of the Jauntlet Namespace to shorten the syntax to call it.
// we want: JMath::Function(), not Jauntlet::JMath::Function().
#include <cstddef>
#include <glm/glm.hpp>

class JMath
{
public:
	// find the distance between two vectors
	static float Distance(glm::vec2 a, glm::vec2 b);
	// find the distance between two vectors
	static float Distance(glm::vec3 a, glm::vec3 b);
};


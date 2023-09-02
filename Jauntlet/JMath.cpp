#include "JMath.h"

float JMath::Distance(glm::vec2 a, glm::vec2 b) {
	return std::sqrt(std::pow(a.x - b.x, 2) + std::pow(a.y - b.y, 2));
}

float JMath::Distance(glm::vec3 a, glm::vec3 b) {
	float dist1 = std::sqrt(std::pow(a.x - b.x, 2) + std::pow(a.y - b.y, 2));
	if (a.z == b.z) { // skip the calculation if z's are the same
		return dist1;
	}
	return std::sqrt(dist1 + std::pow(a.z + b.z, 2));
}
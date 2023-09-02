#include "JMath.h"

float JMath::Distance(glm::vec2 a, glm::vec2 b) {
	return std::sqrt(std::pow(a.x - b.x, 2) + std::pow(a.y - b.y, 2));
}

float JMath::Distance(glm::vec3 a, glm::vec3 b) {
	                           // a^2 + b^2 = c^2                                 // c^2 + z^2 = s^2
	return std::sqrt(std::sqrt(std::pow(a.x - b.x, 2) + std::pow(a.y - b.y, 2)) + std::pow(a.z + b.z, 2));
}
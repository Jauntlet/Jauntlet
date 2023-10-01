#include "JMath.h"

#include <iostream> // REMOVE 

float JMath::Distance(glm::vec2 a, glm::vec2 b) {
	return std::sqrt(std::pow(a.x - b.x, 2) + std::pow(a.y - b.y, 2));
}

float JMath::Distance(glm::vec3 a, glm::vec3 b) {
	                           // a^2 + b^2 = c^2                                 // c^2 + z^2 = s^2
	return std::sqrt(std::sqrt(std::pow(a.x - b.x, 2) + std::pow(a.y - b.y, 2)) + std::pow(a.z + b.z, 2));
}

std::vector<std::string> JMath::Split(std::string string, std::string delimiter) {
	std::vector<std::string> output;
	int delim = string.find(delimiter);

	do {
		output.push_back(string.substr(0, delim));
		string.erase(0, delim + delimiter.length() + 1);
	} while ((delim = string.find(delimiter)) != std::string::npos);
	
	output.push_back(string);
	
	if (output.size() == 1) {
		// most Split functions in other languages returns an empty string at the 1st index if there is no delimiter in the string. This is for consistency. -xm
		output.push_back("");
	}

	return output;
}
std::vector<std::string> JMath::Split(std::string string, char delimiter) {
	std::vector<std::string> output;

	int delim = string.find(delimiter);
	do {
		output.push_back(string.substr(0, delim));
		string.erase(0, delim + 1);
	} while ((delim = string.find(delimiter)) != std::string::npos);
	
	output.push_back(string);
	
	if (output.size() == 1) {
		// most Split functions in other languages returns an empty string at the 1st index if there is no delimiter in the string. This is for consistency. -xm
		output.push_back("");
	}
	return output;
}
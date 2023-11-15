#include "JMath.h"

float JMath::Distance(const glm::vec2& a,const glm::vec2& b) {
	return std::sqrt(std::pow(a.x - b.x, 2) + std::pow(a.y - b.y, 2));
}
float JMath::Distance(const glm::vec3& a,const glm::vec3& b) {
	                           // a^2 + b^2 = c^2                                 // c^2 + z^2 = s^2
	return std::sqrt(std::sqrt(std::pow(a.x - b.x, 2) + std::pow(a.y - b.y, 2)) + std::pow(a.z + b.z, 2));
}

std::vector<std::string> JMath::Split(std::string string,const std::string& delimiter) {
	std::vector<std::string> output;
	
	size_t delim = string.find(delimiter);

	// if no delimiter was found
	if (delim == -1) {
		output.push_back(string);
		// most Split functions in other languages returns an empty string at the 1st index if there is no delimiter in the string. This is for consistency. -xm
		output.push_back("");
		return output;
	}

	do {
		output.push_back(string.substr(0, delim));
		string.erase(0, delim + delimiter.length());
	} while ((delim = string.find(delimiter)) != std::string::npos);
	
	output.push_back(string);

	return output;
}
std::vector<std::string> JMath::Split(std::string string,const char& delimiter) {
	std::vector<std::string> output;

	size_t delim = string.find(delimiter);

	// if no delimiter was found
	if (delim == -1) {
		output.push_back(string);
		// most Split functions in other languages returns an empty string at the 1st index if there is no delimiter in the string. This is for consistency. -xm
		output.push_back("");
		return output;
	}

	do {
		output.push_back(string.substr(0, delim));
		string.erase(0, delim + 1);
	} while ((delim = string.find(delimiter)) != std::string::npos);
	
	// pushes the rest of the string
	output.push_back(string);
	
	return output;
std::string JMath::reverse(const std::string& string) {
	std::string output = "";
	for (int i = string.length() -1; i >= 0; i--) {
		output += string[i];
	}
	return output;
}
}
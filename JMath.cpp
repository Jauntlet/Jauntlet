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
}

std::string JMath::reverse(const std::string& string) {
	std::string output = "";
	for (int i = string.length() -1; i >= 0; i--) {
		output += string[i];
	}
	return output;
}

std::string operator*(std::string a, const int& b) {
	if (b == 0) return "";
	else if (b < 0) {
		a = JMath::reverse(a);
		std::string c = a;
		for (int i = b; i < -1; i++) {
			a += c;
		}
		return a;
	}
	std::string c = a;

	for (int i = b; i > 1; i--) {
		a += c;
	}
	return a;
}
void operator*=(std::string& a, const int& b) {
	if (b == 0) {
		a = "";
		return;
	}
	else if (b < 0) {
		a = JMath::reverse(a);
		std::string c = a;
		for (int i = b; i < -1; i++) {
			a += c;
		}
		return;
	}
	std::string c = a;
	
	for (int i = b; i > 1; i--) {
		a += c;
	}
}

std::string operator-(std::string& a, const int& b) {
	return a.substr(0, a.length() - b);
}
void operator-=(std::string& a, const int& b) {
	a = a.substr(0, a.length() - b);
}
std::string operator-(std::string a, const char b) {
	return a.erase(a.find_last_of(b), 1);
}
void operator-=(std::string& a, const char b) {
	a.erase(a.find_last_of(b), 1);
}
std::string operator-(std::string a, const std::string& b) {
	return a.erase(a.rfind(b), b.length());
}
void operator-=(std::string& a, const std::string& b) {
	a.erase(a.rfind(b), b.length());
}
std::string operator-(std::string a, const char* b) {
	for (int i = 0; b[i] != '\0'; i++) for (int j = a.length() - 1; j > -1; --j) {
		if (a[j] == b[i]) {
			a.erase(j, 1);
			break;
		}
	}
	return a;
}
void operator-=(std::string& a, const char* b) {
	for (int i = 0; b[i] != '\0'; i++) for (int j = a.length() - 1; j > -1; --j) {
		if (a[j] == b[i]) {
			a.erase(j, 1);
			break;
		}
	}
}
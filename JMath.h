/* Purpose: Different math functions optimized for Jauntlet (Jauntlet Math)
 * Main Contributer(s): Xander Mooney
 */
#pragma once
#include <cmath>
#include <cstddef>
#include <glm/glm.hpp>
#include <string>
#include <SDL/SDL_stdinc.h>
#include <vector>

// JMath is purposefully left out of the Jauntlet Namespace to shorten the syntax to call it.
// we want: JMath::Function(), not Jauntlet::JMath::Function(). -xm
class JMath
{
public:
	// find the distance between two vectors
	static float Distance(const glm::vec2& a, const glm::vec2& b);
	// find the distance between two vectors
	static float Distance(const glm::vec3& a, const glm::vec3& b);

	// splits a string based on the provided delimiter, splitting as many times as needed.
	// if no delimiter is found, the 0th element will be the full string and the 1th element will be an empty string.
	static std::vector<std::string> Split(std::string string, const std::string& delimiter);
	// splits a string based on the provided delimiter, splitting as many times as needed.
	// if no delimiter is found, the 0th element will be the full string and the 1th element will be an empty string.
	static std::vector<std::string> Split(std::string string, const char& delimiter);

	// Converts Radians to Degrees
	static float Rad2Deg();
	// Converts Degree to Radians
	static float Deg2Rad();
};
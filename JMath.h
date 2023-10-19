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
	static float Distance(glm::vec2 a, glm::vec2 b);
	// find the distance between two vectors
	static float Distance(glm::vec3 a, glm::vec3 b);
	// splits a string in two, returns as an array of two elements [0] being before the delimiter, [1] being after.
	static std::vector<std::string> Split(std::string string, std::string delimiter);
	static std::vector<std::string> Split(std::string string, char delimiter);

	static float Rad2Deg();
	static float Deg2Rad();
private:
	// used for splitting strings
	static std::string arr[2];
};
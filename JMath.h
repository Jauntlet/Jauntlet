/* Purpose: Different math functions optimized for Jauntlet (Jauntlet Math)
 * Main Contributer(s): Xander Mooney
 */
#pragma once
#include <cmath>
#include <cstddef>
#include <glm/ext/vector_int2.hpp>
#include <glm/ext/vector_float2.hpp>
#include <glm/ext/vector_int3.hpp>
#include <glm/ext/vector_float3.hpp>
#include <glm/ext/vector_int4.hpp>
#include <glm/ext/vector_float4.hpp>
#include <string>
#include <vector>

#ifndef M_PI_2
#define M_PI_2 1.57079632679489661923 // pi/2
#endif

#ifndef Rad2Deg
#define Rad2Deg (180.0f / M_PI)
#endif
#ifndef Deg2Rad
#define Deg2Rad (M_PI / 180.0f)
#endif


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
	static std::vector<std::string> Split(const std::string& string, const std::string& delimiter);
	// splits a string based on the provided delimiter, splitting as many times as needed.
	// if no delimiter is found, the 0th element will be the full string and the 1th element will be an empty string.
	static std::vector<std::string> Split(const std::string& string, const char delimiter);
	// splits a string based on the provided delimiter, splitting as many times as needed.
	// if no delimiter is found, the 0th element will be the full string and the 1th element will be an empty string.
	static void Split(const std::string& string, const char delimiter, std::vector<std::string>& output);

	static std::string reverse(const std::string& string);
};

// copies the string 'b' many times
std::string operator*(std::string a, const int& b);
void operator*=(std::string& a, const int& b);
// subtract 'b' from the length of the string
std::string operator-(std::string& a, const int& b);
void operator-=(std::string& a, const int& b);
// subtract the right-most instance of 'b'
std::string operator-(std::string a, const char b);
void operator-=(std::string& a, const char b);
// subtract the right most instance of the string 'b'
std::string operator-(std::string a, const std::string& b);
void operator-=(std::string& a, const std::string& b);
// subtract the right most instance of each char in 'b'
std::string operator-(std::string a, const char* b);
void operator-=(std::string& a, const char* b);

// allows printing vectors easily
std::ostream& operator<<(std::ostream& a, const glm::vec2& b);
std::ostream& operator<<(std::ostream& a, const glm::ivec2& b);
std::ostream& operator<<(std::ostream& a, const glm::vec3& b);
std::ostream& operator<<(std::ostream& a, const glm::ivec3& b);
std::ostream& operator<<(std::ostream& a, const glm::vec4& b);
std::ostream& operator<<(std::ostream& a, const glm::ivec4& b);

// vector division
glm::vec2 operator/(const glm::vec2& a, const glm::vec2& b);
void operator/=(glm::vec2& a, const glm::vec2& b);
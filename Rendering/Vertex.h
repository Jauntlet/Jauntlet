/* Purpose: Data structures frequently used within the engine.
 * Main Contributer(s): Xander Mooney
 */
#pragma once

#include "../Errors.h"
#include <string>

#if __unix__
#include <cstdint>
#endif

#ifdef OPENGL
#include <GL/glew.h>
#endif

namespace Jauntlet {
struct Position {
	float x;
	float y;
};
struct Color {
	uint8_t r;
	uint8_t g;
	uint8_t b;
	uint8_t a;

	Color() : r(255), g(255), b(255), a(255) { }
	// Do not include # in string.
	Color(std::string hex) : a(255){
		try {
		r=std::stoi(hex.substr(0,2),0,16);
		g=std::stoi(hex.substr(2,2),0,16);
		b=std::stoi(hex.substr(4,2),0,16);
		} catch (...) {
		Jauntlet::fatalError("String " + hex + " is not a hexcode.");
		}
	}
	Color(uint8_t R, uint8_t G, uint8_t B, uint8_t A) : r(R), g(G), b(B), a(A) { }
	Color(uint8_t R, uint8_t G, uint8_t B) : r(R), g(G), b(B), a(255) { }

	void setColor(uint8_t red, uint8_t green, uint8_t blue, uint8_t alpha) {
		r = red;
		g = green;
		b = blue;
		a = alpha;
	}
};
struct UV {
	float u;
	float v;
};

struct Vertex {
	Position position;
	Color color;
	UV uv;

	Vertex() {
		position.x = 0;
		position.y = 0;
		uv.u = 0;
		uv.v = 0;
	}
	// setters
	void setColor(uint8_t r, uint8_t g, uint8_t b, uint8_t a) {
		color.r = r;
		color.g = g;
		color.b = b;
		color.a = a;
	}
	void setPosition(float x, float y) {
		position.x = x;
		position.y = y;
	}
	void setUV(float u, float v) {
		uv.u = u;
		uv.v = v;
	}
};
}

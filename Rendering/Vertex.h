/* Purpose: Data structures frequently used within the engine.
 * Main Contributer(s): Xander Mooney
 */
#pragma once

#include "../Errors.h"
#include <GL/glew.h>
#include <string>

namespace Jauntlet {
struct Position {
	float x;
	float y;
};
struct Color {
	GLubyte r;
	GLubyte g;
	GLubyte b;
	GLubyte a;

	Color() : r(255), g(255), b(255), a(255) { }
	Color(std::string hex) : a(255){
		try {
		r=std::stoi(hex.substr(0,2),0,16);
		g=std::stoi(hex.substr(2,2),0,16);
		b=std::stoi(hex.substr(4,2),0,16);
		} catch (...) {
		Jauntlet::fatalError("String " + hex + " is not a hexcode.");
		}
	}
	Color(GLubyte R, GLubyte G, GLubyte B, GLubyte A) : r(R), g(G), b(B), a(A) { }
	Color(GLubyte R, GLubyte G, GLubyte B) : r(R), g(G), b(B), a(255) { }

	void setColor(GLubyte red, GLubyte green, GLubyte blue, GLubyte alpha) {
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
	void setColor(GLubyte r, GLubyte g, GLubyte b, GLubyte a) {
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

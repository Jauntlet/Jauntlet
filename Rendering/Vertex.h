#pragma once

#include <GL/glew.h>

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

	Color() : r(0), g(0), b(0), a(255) { }
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
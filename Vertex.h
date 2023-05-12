#pragma once

#include <GL/glew.h>

struct Position {
	float x;
	float y;
};
struct Color {
	GLubyte r;
	GLubyte g;
	GLubyte b;
	GLubyte a;
};

struct Vertex {
	 Position position;
	 Color color;
};
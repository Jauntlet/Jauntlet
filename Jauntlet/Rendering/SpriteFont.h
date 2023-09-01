#pragma once

#include<SDL_TTF/SDL_ttf.h>
#include<glm/glm.hpp>
#include<map>
#include"./Vertex.h"
#include "SpriteBatch.h"
#include<cstring>

namespace Jauntlet {
#define FIRST_PRINTABLE_CHAR (char)32
#define LAST_PRINTABLE_CHAR (char)126

struct CharGlyph {
	char character;
	glm::vec4 uvRect;
	glm::vec2 size;
};
// Text alignment
enum class Align {
	LEFT, MIDDLE, RIGHT
};

class SpriteFont
{
public:
	// Empty constructor. Use init();
	SpriteFont() {};
	// Constructs SpriteFont with a starting and ending ascii character
	SpriteFont(const char* font, int size, char startChar, char endChar);
	// Constructs SpriteFont using all ascii characters
	SpriteFont(const char* font, int size) :
		SpriteFont(font, size, FIRST_PRINTABLE_CHAR, LAST_PRINTABLE_CHAR) {
	}

	// initialize a SpriteFont that uses all ascii characters
	void init(const char* font, int size);
	// intialize a SpriteFont with set start and ending ascii characters
	void init(const char* font, int size, char startChar, char endChar);

	// Destroys the font resources
	void dispose();
	
	// Get the height of the font
	int getFontHeight() const {
		return _fontHeight;
	}

	// Measures the dimensions of the text
	glm::vec2 measure(const char* string);
	
	// Draws the spritefont using a spritebatch
	void draw(SpriteBatch& spritebatch, const char* string, glm::vec2 position, glm::vec2 scaling,
		float depth, Color tint, Align alignment = Align::LEFT);
private:
	static std::vector<int>* createRows(glm::ivec4* rects, int rectsLength, int rows, int padding, int& w);

	int _fontHeight;
	int _regStart, _regLength;
	CharGlyph* _glyphs;

	unsigned int _texID;
};
}


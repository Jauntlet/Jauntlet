/* Purpose: Uses .ttf files to be able to load fonts and render them on screen as requested.
 * Main Contributer(s): Xander Mooney
 */
#pragma once

#include <ft2build.h>
#include FT_FREETYPE_H
#include <glm/glm.hpp>
#include <map>
#include <cstring>

#include "../Camera2D.h"
#include "GLSLProgram.h"
#include "SpriteBatch.h"
#include "Vertex.h"

namespace Jauntlet {
#define FIRST_PRINTABLE_CHAR (char)32
#define LAST_PRINTABLE_CHAR (char)126

struct CharGlyph {
	unsigned int TextureID;
	glm::ivec2 Size;
	glm::ivec2 Bearing;
	unsigned int Advance;
};

class SpriteFont
{
public:
	// Empty constructor. Use init();
	SpriteFont() {};

	// initialize a SpriteFont that uses all ascii characters
	void init(Camera2D* camera, const char* font, int size);
	
	// Draws the spritefont using a spritebatch
	void draw(SpriteBatch& spritebatch, std::string string, glm::vec2 position, glm::vec2 scaling,
			  float depth, Color color);

	// We only make a getter because we should not be able to set the fontHeight at any time. This would cause multiple issues when rendering
	// due to how it is setup. -xm

	// Get the height of the font
	int getFontHeight(); 
private:
	int _fontHeight;
	std::map<char, CharGlyph> Characters;

	Camera2D* _camera = nullptr;

	static GLSLProgram _textProgram; 
};
}


/* Purpose: Uses .ttf files to be able to load fonts and render them on screen as requested.
 * Main Contributer(s): Xander Mooney
 */
#pragma once

#include <ft2build.h>
#include FT_FREETYPE_H // This include is for the main FreeType header

#include FT_TRIGONOMETRY_H // Include additional FreeType headers as needed
#include FT_GLYPH_H

#include <glm/glm.hpp>
#include <map>
#include <cstring>

#include "../Rendering/Cameras/Camera2D.h"
#include "SpriteBatch.h"
#include "GLSLProgram.h"

namespace Jauntlet {
#define FIRST_PRINTABLE_CHAR (char)32
#define LAST_PRINTABLE_CHAR (char)126

struct CharGlyph {
	unsigned int TextureID;
	glm::ivec2 Size;
	glm::ivec2 Bearing;
	unsigned int Advance;
};

class TextRenderer
{
public:
	// Empty constructor. Use init();
	TextRenderer(Camera2D* camera, const char* font, int size);
	
	// Begin rendering to the TextRenderer
	void begin();
	void addText(std::string text, glm::vec2 position, glm::vec2 scaling = glm::vec2(1), float depth = 0, Color color = Color());
	// renders the text drawn to screen
	void Render();
private:
	int _fontHeight;
	std::map<char, CharGlyph> Characters;

	Camera2D* _camera = nullptr;
	SpriteBatch _spriteBatch;

	static GLSLProgram _textProgram;
	GLSLProgram* _storedProgram;
};
}


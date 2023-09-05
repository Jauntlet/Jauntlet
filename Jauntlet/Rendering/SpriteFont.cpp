#include <SDL/SDL.h>
#include <cstdio>
#include <iostream>

#include "../Errors.h"
#include "SpriteBatch.h"
#include "SpriteFont.h"

using namespace Jauntlet;

void SpriteFont::init(const char* font, int size) {
	FT_Library ft;
	if (FT_Init_FreeType(&ft)) {
		fatalError("FREETYPE library failed to initialize!");
	}

	FT_Face face;
	if (FT_New_Face(ft, font, 0, &face)) {
		fatalError("FREETYPE failed to load font: " + (std::string)font);
	}
	// setting width to 0 lets the function dynamically determine the width.
	FT_Set_Pixel_Sizes(face, 0, size);
	_fontHeight = size;

	glPixelStorei(GL_UNPACK_ALIGNMENT, 1); // This disables byte-alignment restrictions.

	for (unsigned char c = FIRST_PRINTABLE_CHAR; c < LAST_PRINTABLE_CHAR; c++) {
		if (FT_Load_Char(face, c, FT_LOAD_RENDER)) {
			std::cout << "Failed to load Font Glyph: " << font << " of char " << c << std::endl;
			continue;
		}
		unsigned int texture;
		glGenTextures(1, &texture);
		glBindTexture(GL_TEXTURE_2D, texture);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, face->glyph->bitmap.width, face->glyph->bitmap.rows, 0, GL_RED, GL_UNSIGNED_BYTE, face->glyph->bitmap.buffer);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		CharGlyph character = { texture, glm::ivec2(face->glyph->bitmap.width, face->glyph->bitmap.rows), 
								glm::ivec2(face->glyph->bitmap.width, face->glyph->bitmap_top), face->glyph->advance.x 
		};
		Characters.insert(std::pair<char, CharGlyph>(c, character));
	}
	
	// Clear memory for face and ft
	FT_Done_Face(face);
	FT_Done_FreeType(ft);
}

void SpriteFont::draw(SpriteBatch& spritebatch, std::string string, glm::vec2 position, glm::vec2 scaling,
	float depth, Color tint) {

	glm::vec2 totalPos = position;

	for (auto c = string.begin(); c != string.end(); c++) {
		CharGlyph currentGlyph = Characters[*c];

		if (*c == '\n') {
			totalPos.y += _fontHeight * scaling.y;
			totalPos.x = position.x;
		}

		glm::vec4 destRect(totalPos, currentGlyph.Bearing);

		spritebatch.draw(destRect, { 0,0,1,1 }, currentGlyph.TextureID, 0, { 255,255,255,255 });

		totalPos.x += (currentGlyph.Advance >> 6) * scaling.x;
	}
}
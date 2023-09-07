#include <SDL/SDL.h>
#include <cstdio>
#include <iostream>

#include "../Errors.h"
#include "SpriteBatch.h"
#include "SpriteFont.h"

using namespace Jauntlet;

GLSLProgram SpriteFont::_textProgram;

void SpriteFont::init(Camera2D* camera, const char* font, int size) {
	_fontHeight = size;
	_camera = camera;
	
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	
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

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		CharGlyph character = { texture, glm::ivec2(face->glyph->bitmap.width, face->glyph->bitmap.rows), 
								glm::ivec2(face->glyph->bitmap.width, face->glyph->bitmap_top), static_cast<unsigned int>(face->glyph->advance.x)
		};
		Characters.insert(std::pair<char, CharGlyph>(c, character));
	}
	
	// Clear memory for face and ft
	FT_Done_Face(face);
	FT_Done_FreeType(ft);

	// Setup shader
	if (_textProgram.isLinked) {
		return;
	}

	_textProgram.compileShaders("Shaders/text.vert", "Shaders/text.frag");
	_textProgram.addAttribute("vertexPosition");
	_textProgram.addAttribute("vertexColor");
	_textProgram.addAttribute("vertexUV");
	_textProgram.linkShaders();
}

void SpriteFont::draw(SpriteBatch& spritebatch, std::string string, glm::vec2 position, glm::vec2 scaling,
	float depth, Color tint) {

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	
	// Store last used program, and then use our program.
	GLSLProgram* storedProg = GLSLProgram::currentProgram;
	_textProgram.use();

	GLfloat color[4] = { tint.r, tint.g, tint.b, 1 };
	glUniform4fv(_textProgram.getUniformLocation("fontColor"), 1, color);
	glUniform1i(_textProgram.getUniformLocation("imageTexture"), 0);
	glUniformMatrix4fv(_textProgram.getUniformLocation("Projection"), 1, GL_FALSE, &_camera->getCameraMatrix()[0][0]);

	for (auto c = string.begin(); c != string.end(); c++) {
		CharGlyph currentGlyph = Characters[*c];

		if (*c == '\n') {
			position.y += _fontHeight * scaling.y;
			position.x = position.x;
		}

		float x = position.x; //+ currentGlyph.Bearing.x * scaling.x; 
		float y = position.y - (currentGlyph.Size.y - currentGlyph.Bearing.y) * scaling.y;

		glm::vec4 destRect(x,y, currentGlyph.Bearing);

		spritebatch.draw(destRect, { 0, 0, 1, 1 }, currentGlyph.TextureID, 0, {255,255,255,255});

		position.x += (currentGlyph.Advance >> 6) * scaling.x;
	}

    if (storedProg != nullptr) {
		/*
    		this if statement solves a frequent issue where there is no
    		program currently in use, but we try to use it anyways. this
    		obviously would cause a segmentation fault, as we are now trying to
    		use a null address, and since we always assume use() takes a proper
    		address, we access 0x00000000 and crash the program. technically, we
    		could check this earlier, but this is likely the fastest solution. -jk
    	*/
    	storedProg->use();
	}
}
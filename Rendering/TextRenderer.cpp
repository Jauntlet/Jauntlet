#include <cstdio>
#include <freetype/freetype.h>
#include "../Errors.h"
#include "TextRenderer.h"

using namespace Jauntlet;

GLSLProgram TextRenderer::textShader;

TextRenderer::TextRenderer(const char* font, int size) :
	_fontHeight(size)
{
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
	FT_Set_Pixel_Sizes(face, 0, _fontHeight);

	glPixelStorei(GL_UNPACK_ALIGNMENT, 1); // This disables byte-alignment restrictions.

	for (unsigned char c = FIRST_PRINTABLE_CHAR; c < LAST_PRINTABLE_CHAR; c++) {
		if (FT_Load_Char(face, c, FT_LOAD_RENDER)) {
			error("Failed to load Font Glyph: " + std::to_string(*font) + " of char " + std::to_string(c));
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
	if (!textShader.isLinked) {
		textShader.compileShaders("Shaders/text.vert", "Shaders/text.frag");
		textShader.addAttribute("vertexPosition");
		textShader.addAttribute("vertexColor");
		textShader.addAttribute("vertexUV");
		textShader.linkShaders();
	}
}

void TextRenderer::begin() {
	// Store last used program, and then use our program.
	_storedProgram = GLSLProgram::currentProgram;
	textShader.use();

	_spriteBatch.begin();
}

void TextRenderer::addText(std::string text, glm::vec2 position, glm::vec2 scaling, float depth, Color color) {
	float storedX = position.x;
	for (auto c = text.begin(); c != text.end(); c++) {
		CharGlyph currentGlyph = Characters[*c];

		if (*c == '\n') {
			position.y -= _fontHeight * scaling.y;
			storedX = position.x;
		}

		float x = storedX;
		float y = position.y - (_fontHeight * scaling.y);

		glm::vec4 destRect(x, y, scaling * (glm::vec2)currentGlyph.Bearing);

		_spriteBatch.draw(destRect, { 0, 0, 1, 1 }, currentGlyph.TextureID, 0, color);

		storedX += (currentGlyph.Advance >> 6) * scaling.x;
	}
}
// This is an override for text rendering meant to be used by a UIManager that accepts the UIManagers own spriteBatch, not really meant for use elsewhere -xm
void TextRenderer::addText(SpriteBatch& externalBatch, std::string text, glm::vec2 position, glm::vec2 scaling, float depth, Color color) {
	float storedX = position.x;
	for (auto c = text.begin(); c != text.end(); c++) {
		CharGlyph currentGlyph = Characters[*c];

		if (*c == '\n') {
			position.y -= _fontHeight * scaling.y;
			storedX = position.x;
		}

		float x = storedX;
		float y = position.y - (_fontHeight * scaling.y);

		glm::vec4 destRect(x, y, scaling * (glm::vec2)currentGlyph.Bearing);

		externalBatch.draw(destRect, { 0, 0, 1, 1 }, currentGlyph.TextureID, 0, color);

		storedX += (currentGlyph.Advance >> 6) * scaling.x;
	}
}

void TextRenderer::Render() {
	_spriteBatch.endAndRender();

	if (_storedProgram != nullptr) {
		_storedProgram->use();
	}
}

glm::vec2 TextRenderer::calculateTextSize(std::string text, glm::vec2 scaling) {
	// HACK
	glm::vec2 position = glm::vec2(0); // TODO: FIXME

	float storedX = 0;

	float currentLineMaxY = 0;
	float maxY = 0;
	float maxX = 0;
	for (auto c = text.begin(); c != text.end(); c++) {
		CharGlyph currentGlyph = Characters[*c];

		if (*c == '\n') {
			position.y -= _fontHeight * scaling.y;
			storedX = position.x;

			currentLineMaxY = 0;
			maxY += currentLineMaxY;
		}

		float x = storedX;
		float y = position.y - (_fontHeight * scaling.y);

		currentLineMaxY = y > currentLineMaxY ? y : currentLineMaxY;

		glm::vec4 destRect(x, y, scaling * (glm::vec2)currentGlyph.Bearing);

		storedX += (currentGlyph.Advance >> 6) * scaling.x;

		maxX = storedX > maxX ? storedX : maxX;
	}

	maxY += currentLineMaxY;

	return glm::vec2(maxX, maxY);
}
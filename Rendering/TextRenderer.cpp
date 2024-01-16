#include <cstdio>
#include <freetype/freetype.h>
#include <glm/fwd.hpp>
#include <iostream>
#include "../Errors.h"
#include "TextRenderer.h"

GLSLProgram TextRenderer::textShader;

TextRenderer::TextRenderer(const char* font, int size) :
	_fontHeight(size)
{
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	FT_Library ft;
	if (FT_Init_FreeType(&ft)) {
		Jauntlet::fatalError("FREETYPE library failed to initialize!");
	}

	FT_Face face;
	if (FT_New_Face(ft, font, 0, &face)) {
		Jauntlet::fatalError("FREETYPE failed to load font: " + (std::string)font);
	}
	// setting width to 0 lets the function dynamically determine the width.
	FT_Set_Pixel_Sizes(face, 0, _fontHeight);

	glPixelStorei(GL_UNPACK_ALIGNMENT, 1); // This disables byte-alignment restrictions.

	for (unsigned char c = FIRST_PRINTABLE_CHAR; c < LAST_PRINTABLE_CHAR; c++) {
		if (FT_Load_Char(face, c, FT_LOAD_RENDER)) {
			Jauntlet::error("Failed to load Font Glyph: " + std::to_string(*font) + " of char " + std::to_string(c));
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
	for (char c : text) {
		CharGlyph currentGlyph = Characters[c];

		if (c == '\n') {
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
	for (char c : text) {
		CharGlyph currentGlyph = Characters[c];

		if (c == '\n') {
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
	float lineWidth = 0.0f;

	float maxLineWidth = 0.0f;
	float combinedLineHeights = 0.0f;

	bool multiline = false;
	
	for (char c : text) {
		CharGlyph currentGlyph = Characters[c];

		if (c == '\n') {
			// if lineWidth is larger than maxLineWidth, set maxLineWidth to lineWidth 
			maxLineWidth = lineWidth > maxLineWidth ? lineWidth : maxLineWidth;
			// reset lineWidth
			lineWidth = 0.0f;

			// we are on a new line, just use the max font height
			combinedLineHeights += _fontHeight * 0.5f * scaling.y;

			multiline = true;
		}

		// add on our characters width to our total width for this line
		lineWidth += (currentGlyph.Advance >> 6) * scaling.x;
	}

	// this code is just in case we've never done a new line in the text

	// if we dont have a multiline string then just use the last set lineWidth
	maxLineWidth = multiline ? maxLineWidth : lineWidth;
	// if we dont have a multiline string then just use the default font scaling
	combinedLineHeights = multiline ? combinedLineHeights : _fontHeight * scaling.y;

	// by default we calculate the total as the combined total of the distance between the start of the first glyph to the start of the next glyph
	// at the end we just need the regular glyph width since it doesnt have a glyph after it.
	// hack, doesnt work on multiline //TODO: FIXME
	if (!multiline && Characters.size() > 0) {
		// get the last glyph
		CharGlyph currentGlyph = Characters[Characters.size() - 1];
		// get the space AFTER the character
		float spaceAfterCharacter = ((currentGlyph.Advance >> 6) - currentGlyph.Bearing.x) * scaling.x;
		// subtract that from our max width
		maxLineWidth -= spaceAfterCharacter;
	}

	return glm::vec2(maxLineWidth, combinedLineHeights);
}
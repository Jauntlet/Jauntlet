#include "SpriteFont.h"
#include "../Errors.h"

#include<SDL/SDL.h>
#include<cstring>

using namespace Jauntlet;

#define MAX_TEXTURE_RES 4096

SpriteFont::SpriteFont(const char* font, int size, char startChar, char endChar) {
	init(font, size, startChar, endChar);
}

void SpriteFont::init(const char* font, int size) {
	init(font, size, FIRST_PRINTABLE_CHAR, LAST_PRINTABLE_CHAR);
}

void SpriteFont::init(const char* font, int size, char startChar, char endChar) {
	if (!TTF_WasInit()) {
		TTF_Init();
	}

	TTF_Font* ttfFont = TTF_OpenFont(font, size);

	if (ttfFont == nullptr) {
		fprintf(stderr, "Failed to open TTF font file \"%s\"\n", font);
		fflush(stderr);
		fatalError("");
	}
	_fontHeight = TTF_FontHeight(ttfFont);
	_regStart = startChar;
	_regLength = endChar - startChar + 1;
	
	int padding = size / 8;

	// Measure the dimensions of the regions
	glm::ivec4* glyphRects = new glm::ivec4[_regLength];
	int i = 0, advance;
	for (char charID = startChar; charID <= endChar; charID++) {
		TTF_GlyphMetrics(ttfFont, charID, &glyphRects[i].x, &glyphRects[i].z, &glyphRects[i].y, &glyphRects[i].w, &advance);
		glyphRects[i].z -= glyphRects[i].x;
		glyphRects[i].x = 0;
		glyphRects[i].w -= glyphRects[i].y;
		glyphRects[i].y = 0;
		i++;
	}

	// Find the best partitioning of glyphs
	int rows = 1, width, height, bestWidth = 0, bestHeight = 0, area = MAX_TEXTURE_RES * MAX_TEXTURE_RES, bestRows = 0;
	std::vector<int>* bestPartition = nullptr;
	while (rows <= _regLength) {
		height = rows * (padding + _fontHeight) + padding;
		std::vector<int>* glyphRows = createRows(glyphRects, _regLength, rows, padding, width);

		// Desire a power of 2 texture
		width = closestPow2(width);
		height = closestPow2(height);

		// Make sure the texture is feasible
		if (width > MAX_TEXTURE_RES || height > MAX_TEXTURE_RES) {
			rows++;
			delete[] glyphRows;
			continue;
		}

		// make sure texture isn't beyond max resolution
		if (area >= width * height) {
			// clear partition if used
			if (bestPartition) {
				delete[] bestPartition;
			}
			bestPartition = glyphRows;
			bestWidth = width;
			bestHeight = height;
			bestRows = rows;
			area = bestHeight * bestWidth;
			rows++;
		}
		else {
			delete[] glyphRows;
			break;
		}
	}

	if (!bestPartition) {
		fprintf(stderr, "Failed to Map TTF font %s to a texture. Resolution may be too large.", font);
		fflush(stderr);
		fatalError("");
	}

	// The font data was successfully created and the font is valid. Create the texture now
	glGenTextures(1, &_texID);
	glBindTexture(GL_TEXTURE_2D, _texID);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, bestWidth, bestHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);

	SDL_Color color = { 255, 255, 255, 255 };
	
	// Draw the glyphs
	int lineY = padding;
	for (int y = 0; y < bestRows; y++) {
		int lineX = padding;
		for (size_t x = 0; x< bestPartition[y].size(); x++) {
			int glyphIndex = bestPartition[y][x];

			SDL_Surface* glyphSurface = TTF_RenderGlyph_Blended(ttfFont, (char)(startChar + glyphIndex), color);

			unsigned char* surfacePixels = (unsigned char*)glyphSurface->pixels;

			for (int i = 0; i < (glyphSurface->w * glyphSurface->h * 4); i += 4) {
				surfacePixels[i] = (unsigned char)((float)surfacePixels[i] * (surfacePixels[i + 3] / 255));
				surfacePixels[i + 1] = surfacePixels[i];
				surfacePixels[i + 2] = surfacePixels[i];
			}

			// Saving glyph image and coordinates
			glTexSubImage2D(GL_TEXTURE_2D, 0, lineX, bestHeight - lineY - 1 - glyphSurface->h, glyphSurface->w, glyphSurface->h, GL_BGRA, GL_UNSIGNED_BYTE, glyphSurface->pixels);
			glyphRects[glyphIndex].x = lineX;
			glyphRects[glyphIndex].y = lineY;
			glyphRects[glyphIndex].z = glyphSurface->w;
			glyphRects[glyphIndex].w = glyphSurface->h;

			SDL_FreeSurface(glyphSurface);
			glyphSurface = nullptr;

			lineX += glyphRects[glyphIndex].z + padding;
		}
		lineY += _fontHeight + padding;
	}
	
	// Draw the unsupported glyph
	int rs = padding - 1;
	int* pureWhiteSquare = new int[rs * rs];
	memset(pureWhiteSquare, 0xffffffff, rs * rs * sizeof(int));
	glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, rs, rs, GL_RGBA, GL_UNSIGNED_BYTE, pureWhiteSquare);
	delete[] pureWhiteSquare;
	pureWhiteSquare = nullptr;

	// Default texture parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	// Create sprite batch glyphs
	_glyphs = new CharGlyph[_regLength + 1];
	for (int i = 0; i < _regLength; i++) {
		_glyphs[i].character = (char)(startChar + i);
		_glyphs[i].size = glm::vec2(glyphRects[i].z, glyphRects[i].w);
		_glyphs[i].uvRect = glm::vec4((float)glyphRects[i].x / (float)bestWidth, (float)glyphRects[i].y / (float)bestHeight, (float)glyphRects[i].z / (float)bestWidth, (float)glyphRects[i].w / (float)bestHeight);
	}
	_glyphs[_regLength].character = ' ';
	_glyphs[_regLength].size = _glyphs[0].size;
	_glyphs[_regLength].uvRect = glm::vec4(0, 0, (float)rs / (float)bestWidth, (float)rs / (float)bestHeight);

	glBindTexture(GL_TEXTURE_2D, 0);
	delete[] glyphRects;
	delete[] bestPartition;
	TTF_CloseFont(ttfFont);
}

void SpriteFont::dispose() {
	if (_texID != 0) {
		glDeleteTextures(1, &_texID);
		_texID = 0;
	}
	if (_glyphs) {
		delete[] _glyphs;
		_glyphs = nullptr;
	}
}

glm::vec2 SpriteFont::measure(const char* string) {
	glm::vec2 size(0, _fontHeight);
	float currentWidth = 0;
	for (int i = 0; string[i] != 0; i++) {
		char character = string[i];
		if (character == '\n') {
			size.y += _fontHeight;
			if (size.x < currentWidth) {
				size.x = currentWidth;
			}
			currentWidth = 0;
		}
		else {
			// check for correct glyph
			int glyphIndex = character - _regStart;
			if (glyphIndex < 0 || glyphIndex >= _regLength) {
				glyphIndex = _regLength;
			}
			currentWidth += _glyphs[glyphIndex].size.x;
		}
	}
	if (size.x < currentWidth) {
		size.x = currentWidth;
	}
	return size;
}

void SpriteFont::draw(SpriteBatch& spritebatch, const char* string, glm::vec2 position, glm::vec2 scaling,
	float depth, Color tint, Align alignment/*= Align::LEFT*/) {
	glm::vec2 finalPos = position;

	// Apply alignment
	if (alignment == Align::MIDDLE) {
		finalPos.x -= measure(string).x * scaling.x / 2;
	}
	if (alignment == Align::RIGHT) {
		finalPos.x -= measure(string).x * scaling.x;
	}

	for (int i = 0; string[i] != 0; i++) {
		char character = string[i];
		if (character == '\n') {
			finalPos.x = position.x;
			finalPos.y += _fontHeight * scaling.y;
		}
		else {
			// Check for correct glyph
			int glyphIndex = character - _regStart;
			if (glyphIndex < 0 || glyphIndex >= _regLength) {
				glyphIndex = _regLength;
			}
			// Render the glyph
			glm::vec4 destRect(finalPos, _glyphs[glyphIndex].size * scaling);

			spritebatch.draw(destRect, _glyphs[glyphIndex].uvRect, _texID, depth, tint);
			finalPos.x += _glyphs[glyphIndex].size.x * scaling.x;
		}
	}
}

std::vector<int>* SpriteFont::createRows(glm::ivec4* rects, int rectsLength, int rows, int padding, int& width) {
	// intialize empty
	std::vector<int>* output = new std::vector<int>[rows]();
	int* currentWidth = new int[rows]();
	for (int i = 0; i < rows; i++) {
		currentWidth[i] = padding;
	}

	// loop through all glyphs
	for (int i = 0; i < rectsLength; i++) {
		// FInd row for placement
		int rowIndex = 0;
		for (int rowIndexIndex = 1; rowIndexIndex < rows; rowIndexIndex++) {
			if (currentWidth[rowIndexIndex] < currentWidth[rowIndex]) {
				rowIndex = rowIndexIndex;
			}
		}

		// Add width to that row
		currentWidth[rowIndex] += rects[i].z + padding;
		// Add glyph to the row list
		output[rowIndex].push_back(i);
	}

	// Find the max width
	width = 0;
	for (int i = 0; i < rows; i++) {
		if (currentWidth[i] > width) {
			width = currentWidth[i];
		}
	}

	return output;
}

int SpriteFont::closestPow2(int i) {
	i--;
	int pi = 1;
	while (i > 0) {
		i >>= 1;
		pi <<= 1;
	}
	return pi;
}

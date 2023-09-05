#include <SDL/SDL.h>
#include <cstdio>

#include "../Errors.h"
#include "SpriteBatch.h"
#include "SpriteFont.h"

using namespace Jauntlet;

SpriteFont::SpriteFont(const char* font, int size, char startChar, char endChar) {
	init(font, size);
}

void SpriteFont::init(const char* font, int size) {
	if (!TTF_WasInit()) {
		TTF_Init();
	}

	TTF_Font* ttfFont = TTF_OpenFont(font, size);
	if (ttfFont == nullptr) {
		fprintf(stderr, "Failed to open TTF font %s\n", font);
		fatalError("");
	}

	SDL_Surface* surfaceText = TTF_RenderText_Blended(ttfFont, "Text", {255, 255, 255});
}

//void SpriteFont::draw(SpriteBatch& spritebatch, const char* string, glm::vec2 position, glm::vec2 scaling,
//	float depth, Color tint, Align alignment = Align::LEFT) {
//
//}
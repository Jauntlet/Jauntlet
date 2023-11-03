#include "ImageLoader.h"
#include "../Errors.h"
#include "../IOManager.h"
#include "picoPNG.h"
#include "ResourceManager.h"
#include <SDL/SDL.h>

using namespace Jauntlet;

static std::vector<unsigned char> _out;

GLTexture ImageLoader::loadPNG(std::string filePath) {
	GLTexture texture = {};

	std::vector<unsigned char> in, out;
	unsigned long width, height;

	if (!IOManager::readFileToBuffer(filePath, in)) {
		return loadMissingTexture();
	}

	int errorCode = decodePNG(out, width, height, &(in[0]), in.size());

	if (errorCode != 0) {
		return loadMissingTexture();
	}

	glGenTextures(1, &(texture.id));

	glBindTexture(GL_TEXTURE_2D, texture.id);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, &(out[0]));

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

	// Check for an error with openGL. An error occurs here if OpenGL isn't initialized.
	if (glGetError() != GL_NO_ERROR) {
		fatalError("Attempted to create a texture before a window is created! Please try to grab the texture later in program execution!");
	}
	
	glGenerateMipmap(GL_TEXTURE_2D);

	glBindTexture(GL_TEXTURE_2D, 0);

	texture.width = width;
	texture.height = height;

	return texture;
}

SDL_Surface* ImageLoader::loadPNGtoSurface(std::string filePath) {
	std::vector<unsigned char> in;
	unsigned long width, height;

	if (!IOManager::readFileToBuffer(filePath, in)) {
		fatalError("Failed to load PNG " + filePath + " file to buffer!");
	}

	int errorCode = decodePNG(_out, width, height, &(in[0]), in.size());

	if (errorCode != 0) {
		fatalError("decodePNG failed with error: " + std::to_string(errorCode));
	}

	SDL_Surface* surface = SDL_CreateRGBSurfaceFrom((void*)_out.data(), width, height, 32, width * 4, 0x000000ff, 0x0000ff00, 0x00ff0000, 0xff000000);

	if (surface == nullptr) {
		SDL_Log("Failed to create icon surface: %s", SDL_GetError());
	}

	return surface;
}
void ImageLoader::freeSurface(SDL_Surface* surface) {
	_out.clear();
	SDL_FreeSurface(surface);
}

GLTexture ImageLoader::loadMissingTexture() {
	GLTexture texture = {};

	std::vector<unsigned char> in;

	std::vector<unsigned char> out;
	unsigned long width, height;

	if (!IOManager::readFileToBuffer(ResourceManager::getMissingTexture(), in)) {
		fatalError("Failed to load PNG \"" + ResourceManager::getMissingTexture() + "\" file to buffer!");
	}

	int errorCode = decodePNG(out, width, height, &(in[0]), in.size());

	if (errorCode != 0) {
		fatalError("failed to decode image \"" + ResourceManager::getMissingTexture() + "\" with error: " + std::to_string(errorCode));
	}

	glGenTextures(1, &(texture.id));

	glBindTexture(GL_TEXTURE_2D, texture.id);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, &(out[0]));

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

	glGenerateMipmap(GL_TEXTURE_2D);

	glBindTexture(GL_TEXTURE_2D, 0);

	texture.width = width;
	texture.height = height;

	return texture;
}
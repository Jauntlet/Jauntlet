#include "ImageLoader.h"
#include "../../Errors.h"
#include "../../Filesystems/FileManager.h"
#include <GL/glew.h>
#define STB_IMAGE_IMPLEMENTATION
#define STBI_NO_FAILURE_STRINGS
#include "../../Externals/stb_image.h"

static stbi_uc* _out;

GLTexture ImageLoader::loadPNG(const std::string& filePath) {
	GLTexture texture = {};

	stbi_uc* out;
	int width, height;

	out = stbi_load(filePath.data(), &width, &height, NULL, 0);

	if (out == NULL) {
		return loadMissingTexture();
	}

	glGenTextures(1, &texture.id);

	glBindTexture(GL_TEXTURE_2D, texture.id);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, &(out[0]));

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

	// Check for an error with openGL. An error occurs here if OpenGL isn't initialized.
	if (glGetError() != GL_NO_ERROR) {
		Jauntlet::fatalError("Attempted to create a texture before a window is created! Please try to grab the texture later in program execution!");
	}
	
	glGenerateMipmap(GL_TEXTURE_2D);

	glBindTexture(GL_TEXTURE_2D, 0);

	texture.width = width;
	texture.height = height;

	return texture;
}

SDL_Surface* ImageLoader::loadPNGtoSurface(const std::string& filePath) {
	int width, height;

	_out = stbi_load(filePath.data(), &width, &height, NULL, 0);

	if (_out == NULL) {
		Jauntlet::fatalError("Failed to load surface: \"" + filePath + "\"");
	}

	SDL_Surface* surface = SDL_CreateRGBSurfaceFrom((void*)_out, width, height, 32, width * 4, 0x000000ff, 0x0000ff00, 0x00ff0000, 0xff000000);

	if (surface == nullptr) {
		Jauntlet::error("Failed to create icon Surfae!");
		Jauntlet::fatalError(SDL_GetError());
	}

	return surface;
}
void ImageLoader::freeSurface(SDL_Surface* surface) {
	delete _out;
	_out = nullptr;
	SDL_FreeSurface(surface);
}

GLTexture ImageLoader::loadMissingTexture() {
	GLTexture texture = {};

	stbi_uc* out;
	int width, height;

	out = stbi_load(Jauntlet::ResourceManager::getMissingTexture().data(), &width, &height, NULL, 0);

	if (out == NULL) {
		return loadMissingTexture();
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
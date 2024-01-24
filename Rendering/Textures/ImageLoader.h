/* Purpose: Loads data necessary for rendering an image to memory
 * Main Contributer(s): Xander Mooney
 */
#pragma once

#include "ResourceManager.h"
#include <SDL2/SDL_surface.h>
#include <string>

// Load images
class ImageLoader
{
public:
	// loads a PNG based on the file path.
	static Texture loadPNG(const std::string& filePath);
	// loads a PNG and saves it to a SDL_Surface
	// !!! Remember to free the surface when you are done using it with ImageLoader::freeSurface(SDL_Surface* surface); !!!
	static SDL_Surface* loadPNGtoSurface(const std::string& filePath);
	// Frees stored surface memory 
	// Forgetting this could lead to a memory leak!
	static void freeSurface(SDL_Surface* surface);
private:
	// loads the missing texture file if it exists
	static Texture loadMissingTexture();
};
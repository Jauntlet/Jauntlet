/* Purpose: Loads data necessary for rendering an image to memory
 * Main Contributer(s): Xander Mooney
 */
#pragma once

#include "GLTexture.h"
#include <string>
#include <SDL/SDL.h>

namespace Jauntlet {
// Load images
class ImageLoader
{
public:
	// loads a PNG based on the file path.
	static GLTexture loadPNG(std::string filePath);
	// loads a PNG and saves it to a SDL_Surface
	// !!! Remember to free the surface when you are done using it with ImageLoader::freeSurface(SDL_Surface* surface); !!!
	static SDL_Surface* loadPNGtoSurface(std::string filePath);
	// Frees stored surface memory 
	// Forgetting this could lead to a memory leak!
	static void freeSurface(SDL_Surface* surface);
private:
	// loads the missing texture file if it exists
	static GLTexture loadMissingTexture();
};
}
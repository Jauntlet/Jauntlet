#include <GL/glew.h>
#include <SDL/SDL.h>

#include "Jauntlet.h"

namespace Jauntlet {
	int init() {
		// Initialize SDL
		SDL_Init(SDL_INIT_EVERYTHING);
		// allows for buffer swapping
		SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

		return 0;
	}
}
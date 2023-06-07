#include "Window.h"
#include"Errors.h"

using namespace Jauntlet;

int Window::create(std::string windowName, int screenWidth, int screenHeight, unsigned int currentFlags)
{

	Uint32 flags = SDL_WINDOW_OPENGL;

	if (currentFlags & INVISIBLE) {
		flags |= SDL_WINDOW_HIDDEN;
	}
	if (currentFlags & FULLSCREEN) {
		flags |= SDL_WINDOW_FULLSCREEN;
	}
	if (currentFlags & BORDERLESS) {
		flags |= SDL_WINDOW_BORDERLESS;
	}

	// create the window
	_sdlWindow = SDL_CreateWindow(windowName.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, screenWidth, screenHeight, SDL_WINDOW_OPENGL);
	if (_sdlWindow == nullptr) {
		fatalError("SDL Window could not be created!");
	}

	// Have OpenGL work on the window
	SDL_GLContext glContext = SDL_GL_CreateContext(_sdlWindow);
	if (glContext == nullptr) {
		fatalError("SDL_GL context could not be created!");
	}
	// enable GLEW for older PCs that don't support everything modern
	GLenum error = glewInit();
	if (error != GLEW_OK) {
		fatalError("Could not initialize glew!");
	}

	// tells you your version of OpenGL
	std::printf("*** OpenGL Version: %s ***\n", glGetString(GL_VERSION));

	// sets the background color of window
	glClearColor(0.298f, 0.094f, 0.125f, 1);

	// this turns on VSync (0 = off, 1 = on)
	SDL_GL_SetSwapInterval(0);

	return 0;
}

void Window::swapBuffer() {
	SDL_GL_SwapWindow(_sdlWindow);
}
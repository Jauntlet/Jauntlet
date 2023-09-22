#include "Window.h"
#include "Errors.h"

using namespace Jauntlet;

Window::Window() {
	// Empty
}

int Window::create(std::string windowName, int screenWidth, int screenHeight, unsigned int currentFlags /*= 0*/)
{

	Uint32 flags = SDL_WINDOW_OPENGL;

	if (currentFlags & INVISIBLE) {
		flags |= SDL_WINDOW_HIDDEN;
	}
	if (currentFlags & FULLSCREEN) {
		flags |= SDL_WINDOW_FULLSCREEN_DESKTOP;
	}
	if (currentFlags & BORDERLESS) {
		flags |= SDL_WINDOW_BORDERLESS;
	}
	if (currentFlags & RESIZEABLE) {
		flags |= SDL_WINDOW_RESIZABLE;
	}

	// create the window
	_sdlWindow = SDL_CreateWindow(windowName.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, screenWidth, screenHeight, flags);
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

	// Enables Alpha Blending
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	return 0;
}

void Window::swapBuffer() {
	SDL_GL_SwapWindow(_sdlWindow);
}

void Window::setWindowSize(glm::ivec2 size) {
	_screenWidth = size.x;
	_screenWidth = size.y;
	SDL_SetWindowSize(_sdlWindow, size.x, size.y);
}
void Window::setWindowSize(int width, int height) {
	_screenWidth = width;
	_screenHeight = height;
	SDL_SetWindowSize(_sdlWindow, width, height);
	glViewport(0, 0, _screenWidth, _screenHeight);
}

void Window::toggleFullscreen() {
	bool isFullscreen = SDL_GetWindowFlags(_sdlWindow) & SDL_WINDOW_FULLSCREEN_DESKTOP;

	SDL_SetWindowFullscreen(_sdlWindow, isFullscreen ? 0 : SDL_WINDOW_FULLSCREEN_DESKTOP);
}
void Window::toggleFullscreen(bool fullscreen) {
	SDL_SetWindowFullscreen(_sdlWindow, fullscreen ? SDL_WINDOW_FULLSCREEN_DESKTOP : 0);
}

// get the width of the screen
int Window::getWindowWidth() {
	return _screenWidth; 
}

int Window::getWindowHeight() {
	return _screenHeight; 
}

glm::ivec2 Window::getWindowSize() {
	SDL_GetWindowSize(_sdlWindow, &_screenWidth, &_screenHeight);
	glViewport(0, 0, _screenWidth, _screenHeight);

	return glm::ivec2(_screenWidth, _screenHeight); 
}
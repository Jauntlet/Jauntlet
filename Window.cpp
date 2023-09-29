#include "Window.h"
#include "Errors.h"

#include "Rendering/picoPNG.h"
#include "IOManager.h"

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

	// this turns on VSync (0 = off, 1 = on)
	SDL_GL_SetSwapInterval(0);

	// Enables Alpha Blending
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	return 0;
}

void Window::setBackgroundColor(Color color) {
	glClearColor(color.r / static_cast<GLclampf>(255), color.g / static_cast<GLclampf>(255), color.b / static_cast<GLclampf>(255), 1);
}
void Window::setWindowIcon(std::string filepath) {
	std::vector<unsigned char> out, in;
	unsigned long width, height;

	if (!IOManager::readFileToBuffer(filepath, in)) {
		fatalError("Failed to load PNG " + filepath + " file to buffer!");
	}

	int errorCode = decodePNG(out, width, height, &(in[0]), in.size());

	if (errorCode != 0) {
		fatalError("decodePNG failed with error: " + std::to_string(errorCode));
	}

	SDL_Surface* icon = SDL_CreateRGBSurfaceFrom((void*)out.data(), width, height, 32, width * 4, 0x000000ff, 0x0000ff00, 0x00ff0000, 0xff000000);

	if (icon == nullptr) {
		SDL_Log("Failed to create icon surface: %s", SDL_GetError());
	}

	SDL_SetWindowIcon(_sdlWindow, icon);

	SDL_FreeSurface(icon);
}

void Window::clearScreen() {
	glClearDepth(1.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
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

void Window::resolveWindowSize() {
	SDL_GetWindowSize(_sdlWindow, &_screenWidth, &_screenHeight);
	glViewport(0, 0, _screenWidth, _screenHeight);
}

// get the width of the screen
int Window::getWindowWidth() {
	return _screenWidth; 
}

int Window::getWindowHeight() {
	return _screenHeight; 
}

glm::ivec2 Window::getWindowSize() {
	return glm::ivec2(_screenWidth, _screenHeight); 
}
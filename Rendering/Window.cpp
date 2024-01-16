#include "Window.h"
#include "../Errors.h"
#include "Textures/ImageLoader.h"

Window::Window(std::string windowName, int screenWidth, int screenHeight, unsigned int currentFlags) 
	: 
	_screenHeight(screenHeight), 
	_screenWidth(screenWidth) 
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
		Jauntlet::fatalError("SDL Window could not be created!");
	}

	// Have OpenGL work on the window
	_context = SDL_GL_CreateContext(_sdlWindow);
	if (_context == nullptr) {
		Jauntlet::fatalError("SDL_GL context could not be created!");
	}
	// enable GLEW for older PCs that don't support everything modern
	GLenum error = glewInit();
	if (error != GLEW_OK) {
		Jauntlet::fatalError("Could not initialize glew!");
	}

	// this turns on VSync (0 = off, 1 = on)
	SDL_GL_SetSwapInterval(0);

	// Enables Alpha Blending
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

Window::Window(std::string windowName, Window* sharedWindow, int screenWidth, int screenHeight, unsigned int currentFlags) 
	: 
	_screenHeight(screenHeight), 
	_screenWidth(screenWidth) 
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
		Jauntlet::fatalError("SDL Window could not be created!");
	}

	// Have OpenGL work on the window
	_context = sharedWindow->_context;

	// enable GLEW for older PCs that don't support everything modern
	GLenum error = glewInit();
	if (error != GLEW_OK) {
		Jauntlet::fatalError("Could not initialize glew!");
	}

	// this turns on VSync (0 = off, 1 = on)
	SDL_GL_SetSwapInterval(0);

	// Enables Alpha Blending
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

Window::~Window() {
	SDL_DestroyWindow(_sdlWindow);
	SDL_GL_DeleteContext(_context);
}

void Window::clearScreen() {
	glClearDepth(1.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Window::swapBuffer() {
	SDL_GL_SwapWindow(_sdlWindow);
}

void Window::setDrawTarget() {
	SDL_GL_MakeCurrent(_sdlWindow, _context);
}


void Window::setBackgroundColor(Color color) {
	glClearColor(color.r / static_cast<GLclampf>(255), color.g / static_cast<GLclampf>(255), color.b / static_cast<GLclampf>(255), 1);
}
void Window::setWindowIcon(std::string filepath) {
	SDL_Surface* icon = ImageLoader::loadPNGtoSurface(filepath);

	SDL_SetWindowIcon(_sdlWindow, icon);

	ImageLoader::freeSurface(icon);
}
void Window::setWindowTitle(std::string newTitle) {
	SDL_SetWindowTitle(_sdlWindow, newTitle.c_str());
}
void Window::setWindowSize(glm::ivec2 size) {
	_screenWidth = size.x;
	_screenWidth = size.y;
	resolveWindowSize();
}
void Window::setWindowSize(int width, int height) {
	_screenWidth = width;
	_screenHeight = height;
	resolveWindowSize();
}

void Window::toggleFullscreen() {
	SDL_SetWindowFullscreen(_sdlWindow, isFullscreen() ? 0 : SDL_WINDOW_FULLSCREEN_DESKTOP);
}
void Window::toggleFullscreen(bool fullscreen) {
	SDL_SetWindowFullscreen(_sdlWindow, fullscreen ? SDL_WINDOW_FULLSCREEN_DESKTOP : 0);
}
bool Window::isFullscreen() {
	return SDL_GetWindowFlags(_sdlWindow) & SDL_WINDOW_FULLSCREEN_DESKTOP;
}

glm::ivec2 Window::resolveWindowSize() {
	SDL_GetWindowSize(_sdlWindow, &_screenWidth, &_screenHeight);
	glViewport(0, 0, _screenWidth, _screenHeight);

	return glm::ivec2(_screenHeight, _screenWidth);
}

void Window::forceFocus() {
	SDL_RaiseWindow(_sdlWindow);
}

glm::ivec2 Window::getPosition() const {
	glm::ivec2 output{};
	SDL_GetWindowPosition(_sdlWindow, &output.x, &output.y);
	return output;
}
int Window::getWindowWidth() const {
	return _screenWidth; 
}
int Window::getWindowHeight() const {
	return _screenHeight; 
}
glm::ivec2 Window::getWindowSize() const {
	return glm::ivec2(_screenWidth, _screenHeight); 
}

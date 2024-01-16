#include "Window.h"
#include "../Errors.h"
#include "Textures/ImageLoader.h"

#ifdef VULKAN
#include <SDL2/SDL_vulkan.h>
#endif

using namespace Jauntlet;

Window::Window(std::string windowName, int screenWidth, int screenHeight, unsigned int currentFlags)
	:
	_screenHeight(screenHeight),
	_screenWidth(screenWidth)
{
#ifdef OPENGL
	Uint32 flags = SDL_WINDOW_OPENGL;
#elif VULKAN
	Uint32 flags = SDL_WINDOW_VULKAN;
#endif

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

#ifdef OPENGL
	// Have OpenGL work on the window
	_context = SDL_GL_CreateContext(_sdlWindow);
	if (_context == nullptr) {
		fatalError("SDL_GL context could not be created!");
	}
	// enable GLEW for older PCs that don't support everything modern
	GLenum error = glewInit();
	if (error != GLEW_OK) {
		fatalError("Could not initialize glew!");
	}

	// this turns on VSync (0 = off, 1 = on)
	SDL_GL_SetSwapInterval(0);

	// Enables Alpha Blending
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
#elif VULKAN
	VkApplicationInfo appInfo{};
	appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
	appInfo.pApplicationName = windowName.c_str();
	appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
	appInfo.pEngineName = "Jauntlet";
	appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
	appInfo.apiVersion = VK_API_VERSION_1_0;

	VkInstanceCreateInfo createInfo{};
	createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
	createInfo.pApplicationInfo = &appInfo;

	uint32_t extensionCount = 0;
	SDL_Vulkan_GetInstanceExtensions(_sdlWindow, &extensionCount, nullptr);
	const char** extensions = new const char* [extensionCount];
	SDL_Vulkan_GetInstanceExtensions(_sdlWindow, &extensionCount, extensions);

#ifdef __APPLE__
	std::vector<const char*> requiredExtensions;

	for (uint32_t i = 0; i < extensionCount; ++i) {
		requiredExtensions.push_back(extensions[i]);
	}
	requiredExtensions.emplace_back(VK_KHR_PORTABILITY_ENUMERATION_EXTENSION_NAME);

	createInfo.flags |= VK_INSTANCE_CREATE_ENUMERATE_PORTABILITY_BIT_KHR;
	createInfo.enabledExtensionCount = (uint32_t)requiredExtensions.size();
	createInfo.ppEnabledExtensionNames = requiredExtensions.data();
#endif

	createInfo.enabledExtensionCount = extensionCount;
	createInfo.ppEnabledExtensionNames = extensions;
	createInfo.enabledLayerCount = 0;

	if (vkCreateInstance(&createInfo, nullptr, &_context) != VK_SUCCESS) {
		Jauntlet::fatalError("Failed to create Vulkan instance!");
	}

	delete[] extensions;
#endif
}

Window::Window(std::string windowName, Jauntlet::Window* sharedWindow, int screenWidth, int screenHeight, unsigned int currentFlags)
	: 
	_screenHeight(screenHeight), 
	_screenWidth(screenWidth) 
{
#ifdef OPENGL
	Uint32 flags = SDL_WINDOW_OPENGL;
#elif VULKAN
	Uint32 flags = SDL_WINDOW_VULKAN;
#endif

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

#ifdef OPENGL
	// Have OpenGL work on the window
	_context = sharedWindow->_context;

	// enable GLEW for older PCs that don't support everything modern
	GLenum error = glewInit();
	if (error != GLEW_OK) {
		fatalError("Could not initialize glew!");
	}

	// this turns on VSync (0 = off, 1 = on)
	SDL_GL_SetSwapInterval(0);

	// Enables Alpha Blending
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
#elif VULKAN
	_context = sharedWindow->_context;
#endif
}

Window::~Window() {
#ifdef OPENGL
	SDL_GL_DeleteContext(_context);
#elif VULKAN
	vkDestroyInstance(_context, nullptr);
#endif
	SDL_DestroyWindow(_sdlWindow);
}

void Window::clearScreen() {
#ifdef OPENGL
	glClearDepth(1.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
#endif
}

void Window::swapBuffer() {
#ifdef OPENGL
	SDL_GL_SwapWindow(_sdlWindow);
#endif
}

void Window::setDrawTarget() {
#ifdef OPENGL
	SDL_GL_MakeCurrent(_sdlWindow, _context);
#endif
}


void Window::setBackgroundColor(Color color) {
#ifdef OPENGL
	glClearColor(color.r / static_cast<GLclampf>(255), color.g / static_cast<GLclampf>(255), color.b / static_cast<GLclampf>(255), 1);
#endif
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
#ifdef OPENGL
	glViewport(0, 0, _screenWidth, _screenHeight);
#endif

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

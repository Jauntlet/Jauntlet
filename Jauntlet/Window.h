#pragma once
#include <SDL/SDL.h>
#include <GL/glew.h>
#include <string>
#include <glm/glm.hpp>

namespace Jauntlet {
// We are setting each ENUM to a different bit so that we can use bitwise operators to have multiple flags at once.
// invisible = 001, fullscreen = 010, borderless = 100. This means we can properly combine the flags.
enum WindowFlags { INVISIBLE = 0x1, FULLSCREEN = 0x2, BORDERLESS = 0x4, RESIZEABLE = 0x8 };

class Window
{
public:
	// Creates a window using SDL and openGL
	int create(std::string windowName, int screenWidth, int screenHeight, unsigned int currentFlags = 0);
	
	// Swaps the windows buffer, should be done every draw frame
	void swapBuffer();
	
	// Sets the size of the window
	void setWindowSize(glm::ivec2 size);
	// Sets the size of the window
	void setWindowSize(int width, int height);
	
	// Toggles fullscreen
	void toggleFullscreen();
	// Toggles fullscreen, true = fullscreen
	void toggleFullscreen(bool fullscreen);
	
	// get the width of the window
	int getWindowWidth();
	// get the height of the window
	int getWindowHeight();
	// get the width and height of the window. Can also fix inconsistencies with the window size being calculated incorrectly
	glm::ivec2 getWindowSize();

private:
	SDL_Window* _sdlWindow;
	int _screenWidth, _screenHeight;
};
}
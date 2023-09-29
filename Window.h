#pragma once
#include "Rendering/Vertex.h"

#include <GL/glew.h>
#include <SDL/SDL.h>
#include <glm/glm.hpp>
#include <string>
#include <vector>

namespace Jauntlet {
// We are setting each ENUM to a different bit so that we can use bitwise operators to have multiple flags at once.
// invisible = 001, fullscreen = 010, borderless = 100. This means we can properly combine the flags.
enum WindowFlags { INVISIBLE = 0x1, FULLSCREEN = 0x2, BORDERLESS = 0x4, RESIZEABLE = 0x8 };

class Window
{
public:
	Window();
	// Creates a window using SDL and openGL
	int create(std::string windowName, int screenWidth, int screenHeight, unsigned int currentFlags = 0);
	
	// sets the default color of the window.
	void setBackgroundColor(Color color);
	// set the icon of the window.
	// if your icon does not appear, it could be because it is too large.
	void setWindowIcon(std::string filepath);

	// Clears what is rendered on screen.
	// WARNING: resets ALL window screens
	void clearScreen();
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
	
	// Collects the correct window size
	// Needed when the window is resized.
	void resolveWindowSize();

	// get the width of the window
	int getWindowWidth();
	// get the height of the window
	int getWindowHeight();
	// get the width and height of the window.
	// make sure to run resolveWindowSize() on changes of window size (read docs)
	glm::ivec2 getWindowSize();

private:
	SDL_Window* _sdlWindow;
	int _screenWidth, _screenHeight;
};
}
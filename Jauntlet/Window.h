#pragma once
#include<SDL/SDL.h>
#include<GL/glew.h>
#include<string>

namespace Jauntlet {
// We are setting each ENUM to a different bit so that we can use bitwise operators to have multiple flags at once.
// invisible = 001, fullscreen = 010, borderless = 100. This means we can properly combine the flags.
enum WindowFlags { INVISIBLE = 0x1, FULLSCREEN = 0x2, BORDERLESS = 0x4 };

class Window
{
public:
	// Creates a window using SDL and openGL
	int create(std::string windowName, int screenWidth, int screenHeight, unsigned int currentFlags = 0);
	// Swaps the windows buffer
	void swapBuffer();
	// getters
	int getScreenWidth() { return _screenWidth; }
	int getScreenHeight() { return _screenHeight; }
private:
	SDL_Window* _sdlWindow;
	int _screenWidth, _screenHeight;
};
}
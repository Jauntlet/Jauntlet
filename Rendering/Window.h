/* Purpose: Handles the state of windows
 * Main Contributer(s): Xander Mooney
 */
#pragma once
#include "Vertex.h"
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_video.h>
#include <GL/glew.h>
#include <glm/ext/vector_float2.hpp>
#include <glm/ext/vector_int2.hpp>
#include <string>
#include <vector>

namespace Jauntlet {
// We are setting each ENUM to a different bit so that we can use bitwise operators to have multiple flags at once.
// invisible = 001, fullscreen = 010, borderless = 100. This means we can properly combine the flags.
enum WindowFlags { INVISIBLE = 0x1, FULLSCREEN = 0x2, BORDERLESS = 0x4, RESIZEABLE = 0x8 };

class Window
{
public:
	Window(std::string windowName, int screenWidth, int screenHeight, unsigned int currentFlags);
	// Shared Windows are windows that share context information between eachother.
	// This is a recommended approach for if you are using multiple windows as otherwise both windows would require you to properly
	// set up their information, usually resulting in the copying of a ton of information.
	Window(std::string windowName, Jauntlet::Window* sharedWindow, int screenWidth, int screenHeight, unsigned int currentFlags);
	~Window();

	// Clears what is rendered on screen.
	// WARNING: resets ALL window screens
	static void clearScreen();
	// Swaps the windows buffer, should be done every draw frame
	void swapBuffer();
	// tells SDL to draw to the selected window, only needed if multiple windows exist.
	void setDrawTarget();

	// sets the default color of the window.
	static void setBackgroundColor(Color color);
	// set the icon of the window.
	// if your icon does not appear, it could be because it is too large.
	void setWindowIcon(std::string filepath);
	// changes the window's title.
	void setWindowTitle(std::string newTitle);
	// Sets the size of the window
	void setWindowSize(glm::ivec2 size);
	// Sets the size of the window
	void setWindowSize(int width, int height);
	
	// Toggles fullscreen
	void toggleFullscreen();
	// Toggles fullscreen, true = fullscreen
	void toggleFullscreen(bool fullscreen);
	// Returns true if window is fullscreened
	bool isFullscreen();
	// Collects the correct window size
	// Needed when the window is resized.
	glm::ivec2 resolveWindowSize();
	
	// Forces window to top of view, and locks users control on the window
	void forceFocus();

	// get the width of the window
	// make sure to run resolveWindowSize() on changes of window size (read docs)
	int getWindowWidth() const;
	// get the height of the window
	// make sure to run resolveWindowSize() on changes of window size (read docs)
	int getWindowHeight() const;
	// get the width and height of the window.
	// make sure to run resolveWindowSize() on changes of window size (read docs)
	glm::ivec2 getWindowSize() const;

private:
	SDL_Window* _sdlWindow;
	SDL_GLContext _context;
	int _screenWidth, _screenHeight;
};
}
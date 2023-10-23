#include <GL/glew.h>
#include <SDL/SDL.h>
#include <exception>
#include <iostream>
#include "Jauntlet.h"

namespace Jauntlet {
	int init() {
		std::set_terminate(terminate);

		// Initialize SDL
		SDL_Init(SDL_INIT_EVERYTHING);
		// allows for buffer swapping
		SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

		return 0;
	}

	void terminate() {
		int userOutput = tinyfd_messageBox("Jauntlet has Crashed!",
			"Jauntlet has reached an unhandled exception!\nWould you like to send an error report to the Jauntlet Dev team?",
			"yesno", "error", 1);
		if (userOutput == 1) {
			tinyfd_messageBox("yes", "user pressed yes probably", "ok", "info", 1);
		}
		else {
			tinyfd_messageBox("no", "user pressed no probably", "ok", "info", 1);
		}

		SDL_Quit();
	}
}
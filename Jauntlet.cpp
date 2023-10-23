#include <GL/glew.h>
#include <SDL/SDL.h>
#include <exception>
#include <iostream>
#include "Jauntlet.h"

#if _WIN32
#include <Windows.h>
#endif

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
			try {
				std::rethrow_exception(std::current_exception());
			}
			catch (const std::exception& ex) {
			#if _WIN32
				std::string output = " mailto:ps24xmooney@efcts.us?subject=Jauntlet_Crash&body=";
				output += ex.what();
				ShellExecuteA(NULL, "open", output.c_str(), NULL, NULL, SW_SHOWNORMAL);
			#endif
			}
#if __linux__
			system("xdg-open mailto:ps24xmooney@efcts.us?subject=Jauntlet_Crash&body=" + std::current_exception());
#endif
		}
		else {
			tinyfd_messageBox("no", "user pressed no probably", "ok", "info", 1);
		}

		SDL_Quit();
	}
}
#include <GL/glew.h>
#include <SDL/SDL.h>
#include <exception>
#include <fstream>
#include "IOManager.h"
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
		SDL_Quit();
		int userOutput = tinyfd_messageBox("Jauntlet has Crashed!",
			"Jauntlet has reached an unhandled exception!\n\nWould you like to send an error report to the Jauntlet Dev team?",
			"yesno", "error", 1);
		if (userOutput == 1) {
			try {
				std::rethrow_exception(std::current_exception());
			}
			catch (const std::exception& ex) {
				
				IOManager::createFolder("Logs");
				 
				std::ofstream errorFile;
				errorFile.open("Logs/Latest.log");
				errorFile << typeid(ex).name() << std::endl << ex.what();
				errorFile.close();

				std::string output = "mailto:ps24xmooney@efcts.us?subject=Jauntlet%20Crash%20Report&body=Hello,%20I%20have%20experienced%20a%20crash%20within%20the%20Jauntlet%20Engine.%20Here%20is%20my%20crash%20report:%0A";
				output += typeid(ex).name();
				output += "%0A"; // newline in URLs
				output += ex.what();
			#if _WIN32
				ShellExecuteA(NULL, "open", output.c_str(), NULL, NULL, SW_SHOWNORMAL);
			#elif __linux__
				execl("/usr/bin/xdg-open", "xdg-open", output.c_str(), nullptr);
			#endif
			}
		}
		exit(-1);
	}
	
	int init() {
		std::set_terminate(terminate);

		if (IOManager::createFolder("Logs")) {
			tinyfd_messageBox("Report", "Created folder!", "ok", "error", 1);
		}
		else {
			tinyfd_messageBox("Report", "Failed to create folder!", "ok", "error", 1);
		}

		// Initialize SDL
		SDL_Init(SDL_INIT_EVERYTHING);
		// allows for buffer swapping
		SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

		return 0;
	}
}
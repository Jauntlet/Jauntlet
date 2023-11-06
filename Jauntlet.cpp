#include <GL/glew.h>
#include <SDL/SDL.h>
#include <exception>
#include <fstream>
#include "Filesystems/FileManager.h"
#include "Jauntlet.h"
#include "Errors.h"

namespace Jauntlet {
	
	int init() {
		// We only want to use our termination script on release builds so that we don't intercept crash logging for IDEs,
		// this is actually only useful for Linux developers, where all builds will run the termination script whilst developing,
		// Windows users will find that when running through VS that VS overrides std::set_terminate anyways. -xm
		#ifdef NDEBUG 
		FileManager::createFolder("Logs");
		// open the file, therefore clearing it.
		std::ofstream errorFile;
		errorFile.open("Logs/Latest.log");
		errorFile.close();

		std::set_terminate(Jauntlet::terminate);
		#endif
		
		// Initialize SDL
		SDL_Init(SDL_INIT_EVERYTHING);
		// allows for buffer swapping
		SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

		return 0;
	}
}
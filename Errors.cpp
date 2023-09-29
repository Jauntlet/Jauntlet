#include <SDL/SDL.h>
#include <cstdlib>
#include <iostream>

#include "Errors.h"

// compiling on windows
#ifdef _WIN32

#include <Windows.h>

namespace Jauntlet {
	void fatalError(std::string errorString) {
		
		// test for if we are running with a terminal
		if (GetFileType(GetStdHandle(STD_INPUT_HANDLE)) == FILE_TYPE_CHAR && GetFileType(GetStdHandle(STD_OUTPUT_HANDLE)) == FILE_TYPE_CHAR) {
			std::cout << errorString << std::endl;
			std::cout << "Press ENTER to quit...";
			int tmp;
			std::cin >> tmp;
		}
		
		SDL_Quit();
		exit(-1);
	}
}

// compiling on unix
#elif defined(__unix__)

#include <unistd.h>

namespace Jauntlet {
	void fatalError(std::string errorString) {

		// test for if we are running with a terminal
		if (isatty(fileno(stdin)) && isatty(fileno(stdout))) {
			std::cout << errorString << std::endl;
			std::cout << "Press ENTER to quit...";
			int tmp;
			std::cin >> tmp;
		}
		
		SDL_Quit();
		exit(-1);
	}
}

#else

// unsupported platform; give no error messages and just let it quit.

namespace Jauntlet {
	void fatalError(std::string errorString) {
		// despite us giving an errorString, we cannot show this error message unless we can ensure we are using a terminal.
		SDL_Quit();
		exit(-1);
	}
}

#endif
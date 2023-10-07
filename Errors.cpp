#include <SDL/SDL.h>
#include <cstdlib>
#include <iostream>

#include "Errors.h"

// compiling on windows
#ifdef _WIN32
#include <Windows.h>
#elif defined(__unix__)
#include <unistd.h>
#endif

namespace Jauntlet {
	void fatalError(std::string errorString) {
#ifdef _WIN32
	// test for if we are running with a terminal
	if (GetFileType(GetStdHandle(STD_INPUT_HANDLE)) == FILE_TYPE_CHAR && GetFileType(GetStdHandle(STD_OUTPUT_HANDLE)) == FILE_TYPE_CHAR) {
		std::cout << errorString << std::endl;
		std::cout << "Press ENTER to quit...";
		int tmp;
		std::cin >> tmp;
	}
#elif defined(__unix__)
	// test for if we are running with a terminal
	if (isatty(fileno(stdin)) && isatty(fileno(stdout))) {
		std::cout << errorString << std::endl;
		std::cout << "Press ENTER to quit...";
		int tmp;
		std::cin >> tmp;
	}
#endif // if neither run, then the platform is unsupported. Support for these platforms may come if we get access to PCs with them.
	
	// always quit and exit SDL and the program
	SDL_Quit();
	exit(-1);
	}

	void error(std::string errorString) {
#ifdef _WIN32
	// test for if we are running with a terminal
	if (GetFileType(GetStdHandle(STD_INPUT_HANDLE)) == FILE_TYPE_CHAR && GetFileType(GetStdHandle(STD_OUTPUT_HANDLE)) == FILE_TYPE_CHAR) {
		std::cout << errorString << std::endl;
	}
#elif defined(__unix__)
	// test for if we are running with a terminal
	if (isatty(fileno(stdin)) && isatty(fileno(stdout))) {
		std::cout << errorString << std::endl;
	}
#endif
	}
}
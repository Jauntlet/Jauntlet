#include "Errors.h"

#include <cstdlib>
#include <iostream>
#include <SDL/SDL.h>

namespace Jauntlet {
	void fatalError(std::string errorString) {
		std::cout << errorString << std::endl;
		std::cout << "Press ENTER to quit...";
		int tmp;
		std::cin >> tmp;
		SDL_Quit();
		exit(-1);
	}
}
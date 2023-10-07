#pragma once

#include <string>

namespace Jauntlet {
	// Crashes the game with the reason provided
	extern void fatalError(std::string errorString);
	// Gives a warning to the console / logging system
	extern void error(std::string errorString);
}
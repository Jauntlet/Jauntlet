/* Purpose: Proper error handling across all supported platforms/OS's
 * Main Contributer(s): Xander Mooney / Jack Kennedy
 */
#pragma once

#include <string>
#include <vector>

namespace Jauntlet {
	// Crashes the game with the reason provided
	extern void fatalError(std::string errorString);
	// Gives a warning to the console / logging system
	extern void error(std::string errorString);

	static std::vector<std::string> errors;
}
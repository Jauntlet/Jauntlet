/* Purpose: Proper error handling across all supported platforms/OS's
 * Main Contributer(s): Xander Mooney / Jack Kennedy
 */
#pragma once

#include <string>
#include <vector>

namespace Jauntlet {
	// Crashes the game with the reason provided
	extern void fatalError(const std::string& errorString);
	// Gives a warning to the console / logging system
	extern void error(const std::string& errorString);
	// Logs information to the console / logging system
	extern void log(const std::string& string);

	void terminate();
	void terminate(const std::string& error);

	void dumpLog();
}
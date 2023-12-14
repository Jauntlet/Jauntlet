#include <cstdlib>
#include <iostream>
#include <fstream>
#include "Externals/tinyfiledialogs.h"
#include "Filesystems/FileManager.h"
#include "Errors.h"

// compiling on windows
#ifdef _WIN32
#include <Windows.h>
#elif defined(__unix__)
#include <unistd.h>
#endif

std::vector<std::string> errors;

namespace Jauntlet {
	void fatalError(std::string errorString) {
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
#endif // if neither run, then the platform is unsupported. Support for these platforms may come if we get access to PCs with them.

#ifdef NDEBUG
	terminate(errorString);
#else
	dumpLog();
	exit(-1);
#endif
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
	errors.push_back(errorString);
	}

	void terminate() {
		try {
			std::rethrow_exception(std::current_exception());
			// we dump the log anyways if throwing the exception didn't crash somehow
			dumpLog();
		}
		catch (const std::exception& ex) {
			errors.push_back("Jauntlet Crash information:");
			errors.push_back(typeid(ex).name());
			errors.push_back(ex.what());
			dumpLog();

			int userOutput = tinyfd_messageBox("Jauntlet has Crashed!",
				"Jauntlet has reached an unhandled exception!\n\nWould you like to send an error report to the Jauntlet Dev team?",
				"yesno", "error", 1);
			if (userOutput == 1) {
				std::ofstream errorFile("Logs/Latest.log", std::ios::app);
				errorFile << std::endl << "The cause of the Crash: " << std::endl;
				errorFile << typeid(ex).name() << std::endl << ex.what();
				errorFile.close();

				// Email does not exist yet.
				std::string output = "mailto:nonexistent@gmail.com?subject=Jauntlet%20Crash%20Report&body=Hello,%20I%20have%20experienced%20a%20crash%20within%20the%20Jauntlet%20Engine.%20Below%20I%20attached%20my%20crash%20report:%0A%0AAttach%20the%20file%20located%20at:%20";
				output += FileManager::toAbsoluteFilePath("Logs/Latest.log");
				FileManager::openLink(output);
			}
		}
		exit(-1);
	}
	void terminate(const std::string& error) {
		errors.push_back(error);
		dumpLog();
		int userOutput = tinyfd_messageBox("Jauntlet has Crashed!",
			"Jauntlet has reached an unhandled exception!\n\nWould you like to send an error report to the Jauntlet Dev team?",
			"yesno", "error", 1);
		if (userOutput == 1) {
			std::ofstream errorFile;
			errorFile.open("Logs/Latest.log", std::ios::app);
			errorFile << std::endl << "The cause of the Crash: " << std::endl;
			errorFile << error << std::endl;
			errorFile.close();

			// Email does not exist yet.
			std::string output = "mailto:nonexistent@gmail.com?subject=Jauntlet%20Crash%20Report&body=Hello,%20I%20have%20experienced%20a%20crash%20within%20the%20Jauntlet%20Engine.%20Below%20I%20attached%20my%20crash%20report:%0A%0AAttach%20the%20file%20located%20at:%20";
			output += FileManager::toAbsoluteFilePath("Logs/Latest.log");
			FileManager::openLink(output);
		}
		exit(-1);
	}

	void dumpLog() {
		FileManager::createFolder("Logs");

		std::ofstream errorFile("Logs/Latest.log", std::ios::app);
		for (int i = 0; i < errors.size(); i++) {
			errorFile << errors[0] << std::endl;
			errors.erase(errors.begin());
		}

		errorFile.close();
	}
}
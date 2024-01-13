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

std::vector<std::string> _log;

namespace Jauntlet {
void fatalError(const std::string& errorString) {
	Jauntlet::log(errorString);

	#ifdef NDEBUG
	terminate(errorString);
	#else
	dumpLog();
	exit(-1);
	#endif
}

void error(const std::string& errorString) {
	Jauntlet::log(errorString);
}

void log(const std::string& string) {
#ifdef _WIN32
	// test for if we are running with a terminal
	if (GetFileType(GetStdHandle(STD_INPUT_HANDLE)) == FILE_TYPE_CHAR && GetFileType(GetStdHandle(STD_OUTPUT_HANDLE)) == FILE_TYPE_CHAR) {
		std::cout << string << std::endl;
	}
#elif defined(__unix__)
	// test for if we are running with a terminal
	if (isatty(fileno(stdin)) && isatty(fileno(stdout))) {
		std::cout << string << std::endl;
	}
#endif
	_log.push_back(string);
}

void terminate() {
	try {
		std::rethrow_exception(std::current_exception());
		// we dump the log anyways if throwing the exception didn't crash somehow
		dumpLog();
	}
	catch (const std::exception& ex) {
		_log.push_back("Jauntlet Crash information:");
		_log.push_back(typeid(ex).name());
		_log.push_back(ex.what());
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
			std::string output = "mailto:Jauntlet95@gmail.com?subject=Jauntlet%20Crash%20Report&body=Hello,%20I%20have%20experienced%20a%20crash%20within%20the%20Jauntlet%20Engine.%20Below%20I%20attached%20my%20crash%20report:%0A%0AAttach%20the%20file%20located%20at:%20";
			output += FileManager::toAbsoluteFilePath("Logs/Latest.log");
			FileManager::openLink(output);
		}
	}
	exit(-1);
}
void terminate(const std::string& error) {
	_log.push_back(error);
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
		std::string output = "mailto:Jauntlet95@gmail.com?subject=Jauntlet%20Crash%20Report&body=Hello,%20I%20have%20experienced%20a%20crash%20within%20the%20Jauntlet%20Engine.%20Below%20I%20attached%20my%20crash%20report:%0A%0AAttach%20the%20file%20located%20at:%20";
		output += FileManager::toAbsoluteFilePath("Logs/Latest.log");
		FileManager::openLink(output);
	}
	exit(-1);
}

void dumpLog() {
	FileManager::createFolder("Logs");

	std::ofstream errorFile("Logs/Latest.log", std::ios::app);
	for (int i = 0; i < _log.size(); i++) {
		errorFile << _log[0] << std::endl;
		_log.erase(_log.begin());
	}

	errorFile.close();
}
}
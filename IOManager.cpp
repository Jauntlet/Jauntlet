#include <fstream>
#include <vector>
#include "IOManager.h"

#if _WIN32
#include <Windows.h>
#endif

//REMOVE
#include <iostream>

using namespace Jauntlet;

bool IOManager::readFileToBuffer(std::string filePath, std::vector<unsigned char>& buffer) {
	std::ifstream file(filePath, std::ios::binary);
	if (file.fail()) {
		perror(filePath.c_str());
		return false;
	}

	// go to the end of the file
	file.seekg(0, std::ios::end);

	// Get the file size
	int fileSize = file.tellg();
	// go back to beginning of file
	file.seekg(0, std::ios::beg);

	// this is supposed to remove "header bytes" from the file, however this probably does nothing since we are at the beginning of the file. Leaving this here anyways 
	// incase something breaks way down the line. Never know when it could save someone a few hours -xm
	// fileSize -= file.tellg();

	buffer.resize(fileSize);
	file.read((char*)&(buffer[0]), fileSize);

	file.close();

	return true;
}

bool IOManager::createFolder(std::string folderPath) {
#if _WIN32
	// turns any local paths into an absolute path
	// on Linux this may look like:
	// realPath(folderPath.c_str(), NULL);
	// - xm
	std::string truePath = _fullpath(NULL, folderPath.c_str(), NULL);

	if (CreateDirectory(std::wstring(truePath.begin(), truePath.end()).c_str(), NULL)) {
		// folder is made.
		return true;
	}
	else {
		// folder failed to be made
		return false;
	}
#elif (__unix__)
	
	if (mkdir(folderPath.c_str(), 0700) == -1) {
		return false;
	}
	return true;
#endif
}
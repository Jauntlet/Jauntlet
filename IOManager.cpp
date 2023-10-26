#include <fstream>
#include <vector>
#include "Errors.h"
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

bool IOManager::findFolder(const std::string& folderPath) {
	struct stat sb;
	if (stat(folderPath.c_str(), &sb) == 0) {
		return true;
	}
	return false;
}

bool IOManager::createFolder(const std::string& folderPath) {
#if _WIN32
	std::string truePath = toAbsoluteFilePath(folderPath);
	if (CreateDirectory(std::wstring(truePath.begin(), truePath.end()).c_str(), NULL)) {
		return true;
	}
	return false;
#elif (__unix__)
	if (mkdir(folderPath.c_str(), 0700) == -1) {
		return false;
	}
	return true;
#endif
}

std::string IOManager::toAbsoluteFilePath(const std::string& filePath) {
#if _WIN32
	return _fullpath(NULL, filePath.c_str(), NULL);
#elif (__unix__)
try {
	return realpath(filePath.c_str(), NULL);
} catch (...) {
	fatalError("Tried to find the filepath of a directory that doesn't exist: " + filePath);
}
#endif
}
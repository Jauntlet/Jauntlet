#include <fstream>
#include <vector>
#include "../Errors.h"
#include "FileManager.h"

#if _WIN32
#include <Windows.h>
#endif

using namespace Jauntlet;

const bool FileManager::readFileToBuffer(const std::string& filePath, std::vector<unsigned char>& buffer) {
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

const bool FileManager::findFolder(const std::string& folderPath) {
	struct stat sb;
	if (stat(folderPath.c_str(), &sb) == 0) {
		return true;
	}
	return false;
}
const bool FileManager::createFolder(const std::string& folderPath) {
#if _WIN32
	if (CreateDirectory(std::wstring(folderPath.begin(), folderPath.end()).c_str(), NULL)) {
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

const std::string FileManager::toAbsoluteFilePath(const std::string& filePath) {
// while Windows doesn't care if a file directory exists or not to turn it into a true filepath, Linux very much does.
#if _WIN32
	return _fullpath(NULL, filePath.c_str(), NULL);
#elif (__unix__)
try {
	return realpath(filePath.c_str(), NULL);
} catch (...) {
	fatalError("Tried to find the filepath of a directory that doesn't exist: " + filePath);
	return NULL;
}
#endif
}

const void FileManager::openLink(const std::string& link) {
#if _WIN32
	ShellExecuteA(NULL, "open", link.c_str(), NULL, NULL, SW_SHOWNORMAL);
#elif __linux__
	execl("/usr/bin/xdg-open", "xdg-open", link.c_str(), nullptr);
#endif
}
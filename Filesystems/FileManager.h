/* Purpose: Reads binary files into memory
 * Main Contributer(s): Xander Mooney
 */
#pragma once

#include <string>
#include <vector>

#if _WIN32
#include <Windows.h>
#include <fileapi.h>
#elif __linux__
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#endif


namespace Jauntlet {
class FileManager
{
public:
	// reads files to a data buffer. Returns true if it found the file, false otherwise. The data is returned into the second field.
	static const bool readFileToBuffer(std::string filePath, std::vector<unsigned char>& buffer);
	// Finds the specified folder, returns true if found.
	static const bool findFolder(const std::string& folderPath);
	// Create a folder at designated file path
	static const bool createFolder(const std::string& folderPath);
	// Takes a file/folder path that is localized to the project and makes an absolute path.
	// EX: "Build" -> "C:\Users\[Username]\Project\Build"
	// !!! Causes a crash on Linux if the filepath does not exist !!!
	static const std::string toAbsoluteFilePath(const std::string& filePath);
};
}
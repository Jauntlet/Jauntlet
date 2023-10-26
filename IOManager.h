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
class IOManager
{
public:
	// reads files to a data buffer. Returns true if it found the file, false otherwise. The data is returned into the second field.
	static bool readFileToBuffer(std::string filePath, std::vector<unsigned char>& buffer);
	// Create a folder at designated file path
	static bool createFolder(std::string folderPath);
	// Takes a file path that is localized and makes an absolute path.
	// EX: "Build" -> "C:\Users\[Username]\Build"
	static std::string toAbsoluteFilePath(const std::string& filePath);
};
}
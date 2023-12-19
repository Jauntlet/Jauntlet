/* Purpose: Reads binary files into memory
 * Main Contributer(s): Xander Mooney
 */
#pragma once

#include <string>
#include <vector>
#include "../Audio/AudioSource.h"

#if _WIN32
#include <Windows.h>
#include <fileapi.h>
#elif __linux__
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <cstdint>
#endif


namespace Jauntlet {
class FileManager
{
public:
	// reads files to a data buffer. Returns true if it found the file, false otherwise. The data is returned into the second field.
	static const bool readFileToBuffer(const std::string& filePath, std::vector<unsigned char>& buffer);
	
	static std::vector<char> readWAVFile(const std::string& filePath, Jauntlet::AudioStream& audioStream);

	// Finds the specified folder, returns true if found.
	static const bool findFile(const std::string& filePath);
	// Create a folder at designated file path
	// returns true if folder is successfully created.
	// Most times a folder could not be created if either the path provided is invalid, or the folder already exists.
	static const bool createFolder(const std::string& folderPath);
	
	// Takes a file/folder path that is localized to the project and makes an absolute path.
	// EX: "Build" -> "C:\Users\[Username]\Project\Build"
	// !!! Causes a crash on Linux if the filepath does not exist !!!
	static const std::string toAbsoluteFilePath(const std::string& filePath);

	// Opens a link in the users default browser. 
	static const void openLink(const std::string& link);
private:
	static const int32_t bufferToInt(char* buffer, size_t len);
	static const bool isBigEndian();
};
}
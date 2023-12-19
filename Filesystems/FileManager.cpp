#include <fstream>
#include <vector>
#include "../Errors.h"
#include "FileManager.h"

#if _WIN32
#include <Windows.h>
#elif __linux__
#include <cstring>
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

std::vector<char> FileManager::readWAVFile(const std::string& filePath, Jauntlet::AudioStream& audioStream) {
	std::ifstream file(filePath, std::ios::binary);
	if (!file.is_open()) {
		Jauntlet::error("Could not open WAV \"" + filePath + "\"");
		return std::vector<char>();
	}

	// -- Processing the Header of the WAV to make sure its valid. --
	char buffer[4];
	// reading the word RIFF; the ChunkID
	if (!file.read(buffer, 4) || std::strncmp(buffer, "RIFF", 4) != 0) {
		Jauntlet::error("WAV file \"" + filePath + "\" has an invalid header: could not read the RIFF");
		return std::vector<char>();
	}
	// reading the size of the file; the ChunkSize
	if (!file.read(buffer, 4)) {
		Jauntlet::error("WAV file \"" + filePath + "\" has an invalid header: could not read the size of the file");
		return std::vector<char>();
	}
	// reading the format ("WAVE")
	if (!file.read(buffer, 4) || std::strncmp(buffer, "WAVE", 4) != 0) {
		Jauntlet::error("WAV file \"" + filePath + "\" has an invalid header: could not read the format!");
		return std::vector<char>();
	}
	// reading SubChunk1ID, should be "fmt ", if not, we check for a JUNK section, which can appear in some WAV files.
	if (!file.read(buffer, 4) || std::strncmp(buffer, "fmt ", 4) != 0) {
		if (std::strncmp(buffer, "JUNK", 4) != 0) {
			Jauntlet::error("WAV file \"" + filePath + "\" has an invalid header: could not read fmt/0");
			return std::vector<char>();
		}
		// WAV has a JUNK section, we need to read the size and then promptly discard it.
		if (!file.read(buffer, 4)) {
			Jauntlet::error("WAV file \"" + filePath + "\" has an invalid header: invalid JUNK size.");
			return std::vector<char>();
		}
		int32_t junkSize = bufferToInt(buffer, 4);

		// the junk size is always even, if its odd there is an empty byte at the end.
		if (junkSize % 2 != 0) {
			++junkSize;
		}
		// skip junk section
		file.seekg(junkSize, std::ios::cur);

		if (!file) {
			Jauntlet::error("WAV file \"" + filePath + "\" has an invalid header: file ends during JUNK header");
			return std::vector<char>();
		}

		// retry reading the "fmt "
		if (!file.read(buffer, 4)) {
			Jauntlet::error("WAV file \"" + filePath + "\" has an invalid header: could not read fmt/0");
			return std::vector<char>();
		}
	}
	
	// reads past the "fmt " subchunk (always 16) and AudioFormat PCM (1 unless there is compression)
	file.seekg(6, std::ios::cur);
	
	if (!file) {
		Jauntlet::error("WAV file \"" + filePath + "\" has an invalid header: file ended during fmt and PCM subchunks");
		return std::vector<char>();
	}

	// reading NumChannels
	if (!file.read(buffer, 2)) {
		Jauntlet::error("WAV file \"" + filePath + "\" has an invalid header: could not read the number of channels");
		return std::vector<char>();
	}
	audioStream.channels = bufferToInt(buffer, 2);

	// reading the SampleRate
	if (!file.read(buffer, 4)) {
		Jauntlet::error("WAV file \"" + filePath + "\" has an invalid header: could not read the sample rate");
		return std::vector<char>();
	}
	audioStream.sampleRate = bufferToInt(buffer, 4);

	// read past the byteRate and block align data sections
	file.seekg(6, std::ios::cur);

	if (!file) {
		Jauntlet::error("WAV file \"" + filePath + "\" has an invalid header: file ended during byteRate and block align data sections");
		return std::vector<char>();
	}

	// reading the bitsPerSample
	if (!file.read(buffer, 2)) {
		Jauntlet::error("WAV file \"" + filePath + "\" has an invalid header: could not read bits per sample");
		return std::vector<char>();
	}
	audioStream.bitsPerSample = bufferToInt(buffer, 2);

	// Reading "data" (SubChunk2ID)
	if (!file.read(buffer, 4) || std::strncmp(buffer, "data", 4) != 0) {
		Jauntlet::error("WAV file \"" + filePath + "\" has an invalid header: could not read data chunk header");
		return std::vector<char>();
	}
	// Reading the size of the data
	if (!file.read(buffer, 4)) {
		Jauntlet::error("WAV file \"" + filePath + "\" has an invalid header: could not read the size of the data");
		return std::vector<char>();
	}
	audioStream.size = bufferToInt(buffer, 4);

	// --  Done processing the header --

	// making sure the file is still good to go
	if (file.eof() || file.fail()) {
		Jauntlet::error("WAV file \"" + filePath + "\" has no data to be read.");
		return std::vector<char>();
	}

	// return the WAV data.
	std::vector<char> output(audioStream.size);
	file.read(output.data(), audioStream.size);
	return output;
}

const bool FileManager::findFile(const std::string& filePath) {
	struct stat sb;
	if (stat(filePath.c_str(), &sb) == 0) {
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

// bufferToInt function provided by https://indiegamedev.net/2020/02/15/the-complete-guide-to-openal-with-c-part-1-playing-a-sound/, modified for older compilers
const int32_t FileManager::bufferToInt(char* buffer, size_t len) {
	int32_t a = 0;
	if (!isBigEndian()) {
		std::memcpy(&a, buffer, len);
	}
	else for (size_t i = 0; i < len; ++i) {
		reinterpret_cast<char*>(&a)[3 - i] = buffer[i];
	}
	return a;
}

const bool FileManager::isBigEndian() {
	union {
        uint32_t i;
        char c[4];
    } endian = {0x01020304};

    return endian.c[0] == 1;
}
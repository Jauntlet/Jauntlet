#include <filesystem>
#include <fstream>
#include <vector>

#include "IOManager.h"

using namespace Jauntlet;

bool IOManager::readFileToBuffer(std::string filePath, std::vector<unsigned char>& buffer) {
	std::ifstream file(std::filesystem::canonical(std::filesystem::current_path()).u8string() + "/" + filePath, std::ios::binary);
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
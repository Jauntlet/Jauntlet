#pragma once

#include<vector>

class IOManager
{
public:
	static bool readFileToBuffer(std::string filePath, std::vector<char>& buffer);
};


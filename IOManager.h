/* Purpose: Reads binary files into memory
 * Main Contributer(s): Xander Mooney
 */
#pragma once

#include <string>
#include <vector>

namespace Jauntlet {
class IOManager
{
public:
	// reads files to a data buffer. Returns true if it found the file, false otherwise. The data is returned into the second field.
	static bool readFileToBuffer(std::string filePath, std::vector<unsigned char>& buffer);
};
}
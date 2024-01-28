/* Purpose: Handles all file related operations in Jauntlet. This may be filepaths management, or reading binary files into memory.
 * Main Contributer(s): Xander Mooney
 */
#pragma once

#include <string>
#include <vector>
#include "../Audio/AudioSource.h"
#include <cstdio>
#include <glm/vec2.hpp>
#include "../Rendering/Textures/ResourceManager.h"
#include <SDL2/SDL_surface.h>

#if _WIN32
#include <Windows.h>
#include <fileapi.h>
#elif __linux__
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <cstdint>
#endif

class FileManager
{
public:
	// reads files to a data buffer. Returns true if it found the file, false otherwise. The data is returned into the second field.
	static const bool readFileToBuffer(const std::string& filePath, std::vector<unsigned char>& buffer);
	
	// takes the filepath location for a WAV file and converts it into a vector of chars for audio streaming.
	static std::vector<char> readWAVFile(const std::string& filePath, AudioStream& audioStream);
	// takes the filepath location for an OBJ file and converts it into various info for rendering a 3D object.
	// Returns true on successful read
	static bool readOBJ(const std::string& filePath, std::vector<glm::vec3>& out_vertices, std::vector<unsigned short>& out_indices, std::vector<glm::vec2>& out_uvs, std::vector<glm::vec3>& out_normals);
	
	static bool readGLTF(const std::string& filePath, std::vector<glm::vec3>& out_vertices, std::vector<unsigned short>& out_indices, std::vector<glm::vec2>& out_uvs, std::vector<glm::vec3>& out_normals);

	// loads an image based on the file path for rendering.
	// Supports PNG, JPEG, BMP, PSD, TGA, HDR, PIC, PPM, PGM
	static Texture readImage(const std::string& filePath);

	// loads a PNG and saves it to a SDL_Surface
	// !!! Remember to free the surface when you are done using it with ImageLoader::freeSurface(SDL_Surface* surface); !!!
	static SDL_Surface* loadImageToSurface(const std::string& filePath);
	// Frees stored surface memory 
	// Forgetting to do this after `loadImageToSurface` leads to leaked memory.
	static void freeSurface(SDL_Surface* surface);

	// Finds the specified folder, returns true if found.
	static const bool findFile(const std::string& filePath);
	// Create a folder at designated file path
	// returns true if folder is successfully created.
	// Most times a folder could not be created if either the path provided is invalid, or the folder already exists.
	static const bool createFolder(const std::string& folderPath);
	// Deletes file from storage
	// Returns false if the file does not exist, or the file could not be removed
	static const bool deleteFile(const std::string& filePath);
	
	// Takes a file/folder path that is localized to the project and makes an absolute path
	// EX: "Build" -> "C:\Users\[Username]\Project\Build".
	// Causes a crash on Linux if the filepath does not exist.
	static const std::string toAbsoluteFilePath(const std::string& filePath);

	// Opens a link in the users default browser. 
	static const void openLink(const std::string& link);
private:
	static const int32_t bufferToInt(char* buffer, size_t len);
	static const bool isBigEndian();
};
#include <fstream>
#include <unordered_map>
#include <vector>
#include "../Errors.h"
#include "FileManager.h"
#include "../JMath.h"
#include "../Filesystems/JSON.h"

#define STB_IMAGE_IMPLEMENTATION
#define STBI_NO_FAILURE_STRINGS
#include "../Externals/stb_image.h"
#include <GL/glew.h>

#if _WIN32
#include <Windows.h>
#elif __linux__
#include <cstring>
#endif

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

std::vector<char> FileManager::readWAVFile(const std::string& filePath, AudioStream& audioStream) {
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

// Allows for Vec3s to be put in an unordered map. May be moved to JMath later if needed elsewhere. -xm
struct Vec3Map {
	size_t operator()(const glm::vec3& k) const {
		return std::hash<float>()(k.x) ^ std::hash<float>()(k.y) ^ std::hash<float>()(k.z);
	}
	bool operator()(const glm::vec3& a, const glm::vec3& b) const {
		return a.x == b.x && a.y == b.y && a.z == b.z;
	}
};

bool FileManager::readOBJ(const std::string& filePath, std::vector<glm::vec3>& out_vertices, std::vector<unsigned short>& out_indices, std::vector<glm::vec2>& out_uvs, std::vector<glm::vec3>& out_normals) {
 	std::vector<glm::vec3> vertices, normals;
	std::vector<glm::vec2> uvs;
	std::unordered_map<glm::vec3, unsigned int, Vec3Map, Vec3Map> vertexMap;
	
	std::ifstream file(filePath, std::ios::in);
	if (!file.is_open()) {
		Jauntlet::error("Could not open OBJ \"" + filePath + "\"");
		return false;
	}

	std::string line;
	while (getline(file, line)) {
		// lines that start with a 'v' are a vertex
		if (line[0] == 'v' && line[1] == ' ') {
			std::vector<std::string> split; 
			JMath::Split(line, ' ', split);
			vertices.emplace_back(std::strtof(split[1].data(), NULL), std::strtof(split[2].data(), NULL), std::strtof(split[3].data(), NULL));
		}
		// lines that start with 'vt' are a texture coordinate
		else if (line[0] == 'v' && line[1] == 't') {
			std::vector<std::string> split;
			JMath::Split(line, ' ', split);
			uvs.emplace_back(std::strtof(split[1].data(), NULL), std::strtof(split[2].data(), NULL));
		} 
		// lines that start with 'vn' are normals.
		else if (line[0] == 'v' && line[1] == 'n') {
			std::vector<std::string> split;
			JMath::Split(line, ' ', split);
			normals.emplace_back(std::strtof(split[1].data(), NULL), std::strtof(split[2].data(), NULL), std::strtof(split[3].data(), NULL));
		} 
		// lines that start with 'f' are a face.
		else if (line[0] == 'f') {
			std::vector<std::string> splitSpaces;
			JMath::Split(line.data() + 2, ' ', splitSpaces);
			// make sure there are only three points on a face
			if (splitSpaces.size() == 4) {
				Jauntlet::error("OBJ \"" + filePath + "\" is not triangulated! Please re-export the model whilst triangulated.");
				return false;
			}
			for (std::string& info : splitSpaces) {
				std::vector<std::string> splitSlashes;
				JMath::Split(info, '/', splitSlashes);
				
				unsigned int vertIndex = atoi(splitSlashes[0].data()) - 1;
				auto it = vertexMap.find(vertices[vertIndex]);
				if (it == vertexMap.end() || out_uvs[it->second] != uvs[atoi(splitSlashes[1].data()) - 1.0f]) {
					unsigned int newIndex = static_cast<unsigned int>(out_vertices.size());
					vertexMap[vertices[vertIndex]] = newIndex;

					out_vertices.emplace_back(vertices[vertIndex]);
					out_uvs.emplace_back(uvs[atoi(splitSlashes[1].data()) - 1.0f]);
					out_normals.emplace_back(normals[atoi(splitSlashes[2].data()) - 1.0f]);
					out_indices.emplace_back(newIndex);
				}
				else {
					out_indices.emplace_back(it->second);
				}
			}
		}
	}
	return true;
}

bool FileManager::readGLTF(const std::string& filePath, std::vector<glm::vec3>& out_vertices, std::vector<unsigned short>& out_indices, std::vector<glm::vec2>& out_uvs, std::vector<glm::vec3>& out_normals, unsigned int& out_textureID) {
	JSON::Document doc(filePath.data(), 0);
	JSON::Value root = doc.getRoot();

	// get the first buffer section
	JSON::Value buffers = root["buffers"][0];

	// get information from buffers section
	uint64_t binLength = buffers["byteLength"].toUint();

	std::string binPath = std::string(filePath.data(), filePath.find_last_of('/') != std::string::npos ? filePath.find_last_of('/') + 1 : filePath.length()) + buffers["uri"].toString();
	
	// read binFile into bin vector
	std::ifstream binFile(binPath, std::ios::binary);
	std::vector<char> bin(binLength);
	binFile.read(bin.data(), binLength);
	binFile.close();
	
	JSON::Value attributes = root["meshes"].first()["primitives"].first()["attributes"];
	JSON::Value accessors = root["accessors"];
	JSON::Value bufferViews = root["bufferViews"];

	// store information for the vertex buffers
	JSON::Value vertAccessor = accessors[attributes["POSITION"].toUint()];
	float* vertBuffer = (float*)(bin.data() + bufferViews[vertAccessor["bufferView"].toUint()]["byteOffset"].toUint());

	// same thing as above for uvs and normals, we just don't store the accessors.
	float* uvBuffer = (float*)(bin.data() + bufferViews[accessors[attributes["TEXCOORD_0"].toUint()]["bufferView"].toUint()]["byteOffset"].toUint());
	float* normalBuffer = (float*)(bin.data() + bufferViews[accessors[attributes["NORMAL"].toUint()]["bufferView"].toUint()]["byteOffset"].toUint());

	uint64_t vertCount = vertAccessor["count"].toUint();
	out_vertices.reserve(sizeof(glm::vec3) * vertCount);
	out_uvs.reserve(sizeof(glm::vec2) * vertCount);
	out_normals.reserve(sizeof(glm::vec3) * vertCount);
	for (uint64_t i = 0; i < vertCount; ++i) {
		uint64_t bytePos = i * 3;
		out_vertices.emplace_back(vertBuffer[bytePos], -vertBuffer[bytePos + 2], vertBuffer[bytePos + 1]);
		out_uvs.emplace_back(uvBuffer[i * 2], 1.0f - uvBuffer[i * 2 + 1]);
		out_normals.emplace_back(normalBuffer[bytePos], normalBuffer[bytePos + 1], normalBuffer[bytePos + 2]);
	}

	JSON::Value indiciesAccessor = accessors[root["meshes"].first()["primitives"].first()["indices"].toUint()];
	unsigned short* indicieBuffer = (unsigned short*)(bin.data() + bufferViews[indiciesAccessor["bufferView"].toUint()]["byteOffset"].toUint());

	uint64_t indicieCount = indiciesAccessor["count"].toUint();
	out_indices.reserve(sizeof(unsigned short) * indicieCount);
	for (uint64_t i = 0; i < indicieCount; ++i) {
		out_indices.emplace_back(indicieBuffer[i]);
	}

	// find and load the texture
	JSON::Value imagePath = root["images"].first()["uri"];
	if (imagePath != nullptr) {
		out_textureID = readImage(std::string(filePath.data(), filePath.find_last_of('/') != std::string::npos ? filePath.find_last_of('/') + 1 : filePath.length()) + std::string(root["images"].first()["uri"].toString())).id;
	}

	return true;
}

Texture FileManager::readImage(const std::string& filePath) {
	Texture texture = {};
	int width, height, channels;

	stbi_uc* out = stbi_load(filePath.data(), &width, &height, &channels, 0);

	if (out == NULL) {
		Jauntlet::fatalError("Failed to load image: \"" + filePath + "\"");
	}

	glGenTextures(1, &texture.id);

	glBindTexture(GL_TEXTURE_2D, texture.id);

	if (channels == 4) {
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, out);
	} else if (channels == 3) {
		glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, out);
	} else {
		Jauntlet::fatalError("image \"" + filePath + "\" has an invalid channel count");
	}

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

	// Check for an error with openGL. An error occurs here if OpenGL isn't initialized.
	if (glGetError() != GL_NO_ERROR) {
		Jauntlet::fatalError("Attempted to create a texture before a window is created! Please try to grab the texture later in program execution!");
	}

	glGenerateMipmap(GL_TEXTURE_2D);

	glBindTexture(GL_TEXTURE_2D, 0);

	texture.width = width;
	texture.height = height;

	stbi_image_free(out);
	out = nullptr;

	return texture;
}

static stbi_uc* _out = nullptr;
SDL_Surface* FileManager::loadImageToSurface(const std::string& filePath) {
	int width, height;

	_out = stbi_load(filePath.data(), &width, &height, NULL, 0);

	if (_out == NULL) {
		Jauntlet::fatalError("Failed to load surface: \"" + filePath + "\"");
	}

	SDL_Surface* surface = SDL_CreateRGBSurfaceFrom((void*)_out, width, height, 32, width * 4, 0x000000ff, 0x0000ff00, 0x00ff0000, 0xff000000);

	if (surface == nullptr) {
		Jauntlet::error("Failed to create icon Surfae!");
		Jauntlet::fatalError(SDL_GetError());
	}

	return surface;
}
void FileManager::freeSurface(SDL_Surface* surface) {
	SDL_FreeSurface(surface);
	stbi_image_free(_out);
	_out = nullptr;
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
const bool FileManager::deleteFile(const std::string& filePath) {
	return remove(filePath.c_str());
}

const std::string FileManager::toAbsoluteFilePath(const std::string& filePath) {
// while Windows doesn't care if a file directory exists or not to turn it into a true filepath, Linux very much does.
#if _WIN32
	return _fullpath(NULL, filePath.c_str(), NULL);
#elif (__unix__)
try {
	return realpath(filePath.c_str(), NULL);
} catch (...) {
	Jauntlet::fatalError("Tried to find the filepath of a directory that doesn't exist: " + filePath);
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
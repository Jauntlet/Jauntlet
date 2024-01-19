#pragma once
#include "GLSLProgram.h"
#include "Textures/ImageLoader.h"
#include "../Filesystems/FileManager.h"

class Model
{
public:
	Model(const std::string& filePath, const std::string& texturePath);

	void draw();

	glm::vec3 position = glm::vec3(0);
private:
	unsigned int _vertexArrayID, _textureID;
	unsigned int _vertexBuffer, _uvBuffer;

	std::vector<glm::vec3> _vertices, _normals;
	std::vector<glm::vec2> _uvs;
};


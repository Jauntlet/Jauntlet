#pragma once
#include "GLSLProgram.h"
#include "../Filesystems/FileManager.h"
#include <glm/mat4x4.hpp>

class Model
{
public:
	Model(const std::string& filePath, const std::string& texturePath);
	~Model();

	void draw();

	glm::vec3 position = glm::vec3(0);
private:
	unsigned int _vertexArrayID, _textureID;
	unsigned int _vertexBuffer, _uvBuffer, _indexBuffer;

	std::vector<glm::vec3> _vertices, _normals;
	std::vector<glm::vec2> _uvs;
	std::vector<unsigned short> _indices;
};


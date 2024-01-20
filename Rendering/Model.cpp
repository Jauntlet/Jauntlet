#include "Model.h"
#include <glm/trigonometric.hpp>
#include <glm/ext/matrix_transform.hpp>
#include "../Errors.h"

Model::Model(const std::string& filePath, const std::string& texturePath) {
	if (!FileManager::readOBJ(filePath, _vertices, _indices, _uvs, _normals)) {
		Jauntlet::fatalError("Failed to load model \"" + filePath + "\"");
	}

	glGenVertexArrays(1, &_vertexArrayID);
	glBindVertexArray(_vertexArrayID);

	glGenBuffers(1, &_vertexBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, _vertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, _vertices.size() * sizeof(glm::vec3), &_vertices[0], GL_STATIC_DRAW);

	glGenBuffers(1, &_uvBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, _uvBuffer);
	glBufferData(GL_ARRAY_BUFFER, _uvs.size() * sizeof(glm::vec2), &_uvs[0], GL_STATIC_DRAW);

	glGenBuffers(1, &_indexBuffer);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _indexBuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, _indices.size() * sizeof(unsigned short), &_indices[0], GL_STATIC_DRAW);

	_textureID = ImageLoader::loadPNG(texturePath).id;

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);
}
Model::~Model() {
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glDeleteBuffers(1, &_vertexBuffer);
	glDeleteBuffers(1, &_uvBuffer);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glDeleteBuffers(1, &_indexBuffer);
}

void Model::draw() {
	glUniform3f(GLSLProgram::currentProgram->getUniformLocation("modelPosition"), position.x, position.y, position.z);

	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);

	glBindBuffer(GL_ARRAY_BUFFER, _vertexBuffer);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

	glBindBuffer(GL_ARRAY_BUFFER, _uvBuffer);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);

	glBindTexture(GL_TEXTURE_2D, _textureID);
	glDrawElements(GL_TRIANGLES, _indices.size(), GL_UNSIGNED_SHORT, (void*)0);
	//glDrawArrays(GL_TRIANGLES, 0, _vertices.size());

	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
}
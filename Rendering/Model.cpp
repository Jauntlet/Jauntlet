#include "Model.h"
#include <glm/trigonometric.hpp>
#include <glm/ext/matrix_transform.hpp>
#include "../Errors.h"

Model::Model(ModelType type, const std::string& filePath, const std::string& texturePath) {
	if (type == ModelType::OBJ && !FileManager::readOBJ(filePath, _vertices, _indices, _uvs, _normals)) {
		Jauntlet::fatalError("Failed to load model \"" + filePath + "\"");
	}
	else if (type == ModelType::GLTF && !FileManager::readGLTF(filePath, _vertices, _indices, _uvs, _normals, _textureID)) {
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

	if (_textureID == 0) {
		_textureID = FileManager::readImage(texturePath).id;
	}

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
	// currently we calculate the modelTransform every frame which can be pretty intensive,
	// however making it only calculate when the position/rotation changes requires us to make both
	// members private: something we were pretty proud of not doing until this point. 
	// This will have to be discussed soon -xm
	_modelTransform = glm::translate(glm::mat4(1.0f), position) * glm::toMat4(rotation);
	glUniformMatrix4fv(GLSLProgram::currentProgram->getUniformLocation("modelTransform"), 1, GL_FALSE, &_modelTransform[0][0]);

	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);

	glBindBuffer(GL_ARRAY_BUFFER, _vertexBuffer);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

	glBindBuffer(GL_ARRAY_BUFFER, _uvBuffer);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);

	glBindTexture(GL_TEXTURE_2D, _textureID);
	glDrawElements(GL_TRIANGLES, _indices.size(), GL_UNSIGNED_SHORT, (void*)0);

	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
}

void Model::EulerToRotation(const glm::vec3& EulerAngle) {
	rotation = glm::quat(EulerAngle);
}
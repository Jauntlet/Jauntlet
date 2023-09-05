#include <cstddef>

#include "Sprite.h"
#include "Vertex.h"
#include "ResourceManager.h"

using namespace Jauntlet;

Sprite::Sprite(float x, float y, float width, float height, std::string texturePath) : _x(x), _y(y), _width(width), _height(height), _vboID(0) {
	_texture = ResourceManager::getTexture(texturePath);

	if (_vboID == 0) {
		glGenBuffers(1, &_vboID);
	}

	Vertex vertexData[6] = {};

	// First triangle
	vertexData[0].setPosition(x + width, y + height);
	vertexData[0].setUV(1, 1);

	vertexData[1].setPosition(x, y + height);
	vertexData[1].setUV(0, 1);

	vertexData[2].setPosition(x, y);
	vertexData[2].setUV(0, 0);

	// second triangle
	vertexData[3].setPosition(x, y);
	vertexData[3].setUV(0, 0);

	vertexData[4].setPosition(x + width, y);
	vertexData[4].setUV(1, 0);

	vertexData[5].setPosition(x + width, y + height);
	vertexData[5].setUV(1, 1);

	for (int i = 0; i < 6; i++)
	{
		vertexData[i].setColor(255, 0, 255, 255);
	}

	vertexData[1].setColor(0, 0, 255, 255);

	vertexData[4].setColor(0, 255, 0, 255);

	glBindBuffer(GL_ARRAY_BUFFER, _vboID);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertexData), vertexData, GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

// this is a destructor
Sprite::~Sprite() {
	if (_vboID != 0) {
		glDeleteBuffers(1, &_vboID);
	}
}

void Sprite::draw() {

	// bind information
	glBindTexture(GL_TEXTURE_2D, _texture.id);
	glBindBuffer(GL_ARRAY_BUFFER, _vboID);
	

	glDrawArrays(GL_TRIANGLES, 0, 6);

	// unbind information (don't unbind the texture)
	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
	glDisableVertexAttribArray(2);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
}
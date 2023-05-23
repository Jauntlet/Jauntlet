#include "Sprite.h"
#include "Vertex.h"
#include "ResourceManager.h"

#include<cstddef>

Sprite::Sprite() : _x(0), _y(0), _height(0), _width(0), _vboID(0) {
	
}

// this is a destructor
Sprite::~Sprite() {
	if (_vboID != 0) {
		glDeleteBuffers(1, &_vboID);
	}
}

void Sprite::init(float x, float y, float width, float height, std::string texturePath) {
	_x = x;
	_y = y;
	_width = width;
	_height = height;
	
	_texture = ResourceManager::getTexture(texturePath);

	if (_vboID == 0) {
		glGenBuffers(1, &_vboID);
	}

	Vertex vertexData[6] = {};

	// yeah the vertexes for the sprite are hardcoded. Feel free to try and use for loops or something I am small brain
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

void Sprite::draw() {

	// bind information
	glBindTexture(GL_TEXTURE_2D, _texture.id);
	glBindBuffer(GL_ARRAY_BUFFER, _vboID);
	glEnableVertexAttribArray(0);
	
	// Position attribute pointer
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, position));
	// Color attribute pointer
	glVertexAttribPointer(1, 4, GL_UNSIGNED_BYTE, GL_TRUE, sizeof(Vertex), (void*)offsetof(Vertex, color));
	// UV attribute pointer
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, uv));

	glDrawArrays(GL_TRIANGLES, 0, 6);

	// unbind information (don't unbind the texture)
	glDisableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}
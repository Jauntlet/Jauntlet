#include<algorithm>

#include "SpriteBatch.h"
using namespace Jauntlet;

SpriteBatch::SpriteBatch() {
	// Empty
}

void SpriteBatch::init() { 
	createVertexArray();
}

void SpriteBatch::begin(GlyphSortType sortType /* = GlyphSortType::TEXTURE*/) {
	_sortType = sortType;
	_renderBatches.clear();
	_glyphs.clear();
}

void SpriteBatch::end() { 
	// Set up pointers for fast sorting
	_glyphPointers.resize(_glyphs.size());

	for (int i = 0; i < _glyphs.size(); i++) {
		_glyphPointers[i] = &_glyphs[i];
	}

	sortGlyphs();
	createRenderBatches();
}
void SpriteBatch::draw(const glm::vec4& destRect, const glm::vec4& uvRect, GLuint texture, float depth, const Color& color) { 

	_glyphs.emplace_back(destRect, uvRect, texture, depth, color);
}
void SpriteBatch::renderBatch() { 

	glBindVertexArray(_vaoID);

	for (int i = 0; i < _renderBatches.size(); i++) {
		glBindTexture(GL_TEXTURE_2D, _renderBatches[i].texture);

		glDrawArrays(GL_TRIANGLES, _renderBatches[i].offset, _renderBatches[i].numVertices);
	}

	glBindVertexArray(0);
}

void SpriteBatch::createRenderBatches() {
	std::vector <Vertex> vertices;
	vertices.resize(_glyphPointers.size() * 6);

	if (_glyphPointers.empty()) {
		return;
	}

	int cv = 0; // current vertex

	_renderBatches.emplace_back(0, 6, _glyphPointers[0]->texture);
	vertices[cv++] = _glyphPointers[0]->topLeft;
	vertices[cv++] = _glyphPointers[0]->bottomLeft;
	vertices[cv++] = _glyphPointers[0]->bottomRight;
	vertices[cv++] = _glyphPointers[0]->bottomRight;
	vertices[cv++] = _glyphPointers[0]->topRight;
	vertices[cv++] = _glyphPointers[0]->topLeft;

	// cg = current Glyph
	for (int cg = 1; cg < _glyphPointers.size(); cg++) {
		if (_glyphPointers[cg]->texture != _glyphPointers[cg - 1]->texture) {
			_renderBatches.emplace_back(cv, 6, _glyphPointers[cg]->texture);
		}
		else {
			_renderBatches.back().numVertices += 6;
		}

		vertices[cv++] = _glyphPointers[cg]->topLeft;
		vertices[cv++] = _glyphPointers[cg]->bottomLeft;
		vertices[cv++] = _glyphPointers[cg]->bottomRight;
		vertices[cv++] = _glyphPointers[cg]->bottomRight;
		vertices[cv++] = _glyphPointers[cg]->topRight;
		vertices[cv++] = _glyphPointers[cg]->topLeft;
	}

	glBindBuffer(GL_ARRAY_BUFFER, _vboID);
	// orphan the buffer (basically kill it)
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), nullptr, GL_DYNAMIC_DRAW);
	// upload the data
	glBufferSubData(GL_ARRAY_BUFFER, 0, vertices.size() * sizeof(Vertex), vertices.data());

	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void SpriteBatch::createVertexArray() {

	if (_vaoID == 0) {
		glGenVertexArrays(1, &_vaoID);
	}

	glBindVertexArray(_vaoID);

	if (_vboID == 0) {
		glGenBuffers(1, &_vboID);
	}

	glBindBuffer(GL_ARRAY_BUFFER, _vboID);

	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);

	// Position attribute pointer
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, position));
	// Color attribute pointer
	glVertexAttribPointer(1, 4, GL_UNSIGNED_BYTE, GL_TRUE, sizeof(Vertex), (void*)offsetof(Vertex, color));
	// UV attribute pointer
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, uv));

	glBindVertexArray(0);
}

void SpriteBatch::sortGlyphs() {
	
	switch (_sortType) {
	case GlyphSortType::BACK_TO_FRONT:
		std::stable_sort(_glyphPointers.begin(), _glyphPointers.end(), compareBackToFront);
		break;
	case GlyphSortType::FRONT_TO_BACK:
		std::stable_sort(_glyphPointers.begin(), _glyphPointers.end(), compareFrontToBack);
		break;
	case GlyphSortType::TEXTURE:
		std::stable_sort(_glyphPointers.begin(), _glyphPointers.end(), compareTexture);
		break;
	}
}

bool SpriteBatch::compareFrontToBack(Glyph* a, Glyph* b) {
	return (a->depth < b->depth);
}
bool SpriteBatch::compareBackToFront(Glyph* a, Glyph* b) {
	return (a->depth > b->depth);
}
bool SpriteBatch::compareTexture(Glyph* a, Glyph* b) {
	return (a->texture < b->texture);
}
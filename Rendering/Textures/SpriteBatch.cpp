#include <algorithm>
#include <math.h>
#include "SpriteBatch.h"
#include "../../Errors.h"

Glyph::Glyph(const glm::vec4& DestRect, const glm::vec4& UvRect, unsigned int Texture, float Depth, const Color& Color) :
	texture(Texture), depth(Depth) {
	topLeft.color = Color;
	topLeft.setPosition(DestRect.x, DestRect.y + DestRect.w);
	topLeft.setUV(UvRect.x, UvRect.y + UvRect.w);

	bottomLeft.color = Color;
	bottomLeft.setPosition(DestRect.x, DestRect.y);
	bottomLeft.setUV(UvRect.x, UvRect.y);

	bottomRight.color = Color;
	bottomRight.setPosition(DestRect.x + DestRect.z, DestRect.y);
	bottomRight.setUV(UvRect.x + UvRect.z, UvRect.y);

	topRight.color = Color;
	topRight.setPosition(DestRect.x + DestRect.z, DestRect.y + DestRect.w);
	topRight.setUV(UvRect.x + UvRect.z, UvRect.y + UvRect.w);
}
Glyph::Glyph(const glm::vec4& DestRect, const glm::vec4& UvRect, float radians, unsigned int Texture, float Depth, const Color& Color) :
	texture(Texture), depth(Depth) {
	glm::vec2 halfDims(DestRect.z * 0.5f, DestRect.w * 0.5f);

	glm::vec2 tl(rotatePoint(-halfDims.x, halfDims.y, radians) + halfDims);
	glm::vec2 bl(rotatePoint(-halfDims.x, -halfDims.y, radians) + halfDims);
	glm::vec2 tr(tl.x + DestRect.z * cos(radians), tl.y + DestRect.z * sin(radians));
	glm::vec2 br(bl.x + DestRect.z * cos(radians), bl.y + DestRect.z * sin(radians));

	topLeft.color = Color;
	topLeft.setPosition(DestRect.x + tl.x, DestRect.y + tl.y);
	topLeft.setUV(UvRect.x, UvRect.y + UvRect.w);

	bottomLeft.color = Color;
	bottomLeft.setPosition(DestRect.x + bl.x, DestRect.y + bl.y);
	bottomLeft.setUV(UvRect.x, UvRect.y);

	bottomRight.color = Color;
	bottomRight.setPosition(DestRect.x + br.x, DestRect.y + br.y);
	bottomRight.setUV(UvRect.x + UvRect.z, UvRect.y);

	topRight.color = Color;
	topRight.setPosition(DestRect.x + tr.x, DestRect.y + tr.y);
	topRight.setUV(UvRect.x + UvRect.z, UvRect.y + UvRect.w);
}

glm::vec2 Glyph::rotatePoint(float x, float y, float angle) {
	return glm::vec2(x * cos(angle) - y * sin(angle), x * sin(angle) + y * cos(angle));
}

SpriteBatch::SpriteBatch() {
	createVertexArray();
}

void SpriteBatch::begin(GlyphSortType sortType) {
	_sortType = sortType;
	_renderBatches.clear();
	_glyphs.clear();
}

void SpriteBatch::draw(const glm::vec4& destRect, const glm::vec4& uvRect, unsigned int texture, float depth, const Color& color) {
	_glyphs.emplace_back(destRect, uvRect, texture, depth, color);
}
void SpriteBatch::draw(const glm::vec4& destRect, unsigned int texture, float depth, const Color& color) {
	_glyphs.emplace_back(destRect, glm::vec4(0,0,1,1), texture, depth, color);
}
void SpriteBatch::draw(const glm::vec4& destRect, const glm::vec4& uvRect, float angle, unsigned int texture, float depth, const Color& color) {
	_glyphs.emplace_back(destRect, uvRect, angle, texture, depth, color);
}
void SpriteBatch::draw(const glm::vec4& destRect, float angle, unsigned int texture, float depth, const Color& color) {
	_glyphs.emplace_back(destRect, glm::vec4(0, 0, 1, 1), angle, texture, depth, color);
}
void SpriteBatch::draw(const glm::vec4& destRect, const glm::vec4& uvRect, const glm::vec2& angle, unsigned int texture, float depth, const Color& color) {
	_glyphs.emplace_back(destRect, uvRect, atan2(angle.y, angle.x), texture, depth, color);
}
void SpriteBatch::draw(const glm::vec4& destRect, const glm::vec2& angle, unsigned int texture, float depth, const Color& color) {
	_glyphs.emplace_back(destRect, glm::vec4(0, 0, 1, 1), atan2(angle.y, angle.x), texture, depth, color);
}

void SpriteBatch::draw(Glyph glyph) {
	_glyphs.emplace_back(glyph);
}

void SpriteBatch::end() { 
	// Set up pointers for fast sorting
	_glyphPointers.resize(_glyphs.size());

	for (int i = 0; i < _glyphs.size(); ++i) {
		_glyphPointers[i] = &_glyphs[i];
	}

	sortGlyphs();
	createRenderBatches();
}

void SpriteBatch::render() { 

	glBindVertexArray(_vaoID);

	for (int i = 0; i < _renderBatches.size(); ++i) {
		glBindTexture(GL_TEXTURE_2D, _renderBatches[i].texture);

		glDrawArrays(GL_TRIANGLES, _renderBatches[i].offset, _renderBatches[i].numVertices);
	}

	glBindVertexArray(0);
}

void SpriteBatch::endAndRender() {
	end();
	render();
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

	if (glGetError() != GL_NO_ERROR) {
		Jauntlet::fatalError("Tried to initialize a SpriteBatch before a window is created! Please reorder your initialization order!");
	}

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
		case GlyphSortType::NONE:
			break;
		default:
			Jauntlet::error("invalid sort type found in spritebatch!");
			break;
	}
}

bool SpriteBatch::compareFrontToBack(Glyph* a, Glyph* b) {
	return (a->depth < b->depth);
}
bool SpriteBatch::compareBackToFront(Glyph* a, Glyph* b) {
	return (a->depth > b->depth);
}

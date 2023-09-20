#pragma once

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <vector>

#include "Vertex.h"

namespace Jauntlet {
// defines the order for sprites to be drawn
enum class GlyphSortType {
	NONE, FRONT_TO_BACK, BACK_TO_FRONT, TEXTURE
};
// stores the information needed to render a texture
class Glyph {
public:
	Glyph() {};
	Glyph(const glm::vec4& DestRect, const glm::vec4& UvRect, GLuint Texture, float Depth, const Color& Color) :
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
	};

	GLuint texture;
	float depth;

	Vertex topLeft;
	Vertex bottomLeft;
	Vertex topRight;
	Vertex bottomRight;
};
// stores a texture, and information about rendering it
class RenderBatch {
public:
	RenderBatch(GLuint Offset, GLuint NumVertices, GLuint Texture) : offset(Offset), numVertices(NumVertices), texture(Texture) {}
	GLuint offset;
	GLuint numVertices;
	GLuint texture;
};
// combines information for loading textures into batches to send to the GPU at once.
class SpriteBatch
{
public:
	SpriteBatch();
	// initializes the spritebatch
	void init();
	// Opens the spritebatch to be able to add textures to it
	void begin(GlyphSortType sortType = GlyphSortType::NONE);
	// adds a texture and its information to the spritebatch
	void draw(const glm::vec4& destRect, const glm::vec4& uvRect, GLuint texture, float depth, const Color& color = Color(255, 255, 255));
	// closes and sorts the spritebatch
	void end();
	// renders the contents of the spritebatch to screen
	void renderBatch();
	// closes and renders the contents of the spritebatch to screen
	void endAndRenderBatch();
private: 
	void createRenderBatches();
	void createVertexArray();
	void sortGlyphs();

	static bool compareFrontToBack(Glyph* a, Glyph* b);
	static bool compareBackToFront(Glyph* a, Glyph* b);
	static bool compareTexture(Glyph* a, Glyph* b);

	GLuint _vboID = 0;
	GLuint _vaoID = 0;

	GlyphSortType _sortType = GlyphSortType::NONE;

	std::vector<Glyph> _glyphs;
	// The reason why we have a seperate vector for pointing to the glyphs, is so that we can easily sort them to be rendered, without the negative performance
	// boost of having to allocate and unallocate memory
	std::vector<Glyph*> _glyphPointers;

	std::vector<RenderBatch> _renderBatches;
};
}
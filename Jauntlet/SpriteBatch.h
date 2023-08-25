#pragma once
#include<GL/glew.h>
#include<glm/glm.hpp>
#include<vector>

#include"Vertex.h"

namespace Jauntlet {
// defines the order for sprites to be drawn
enum class GlyphSortType {
	NONE, FRONT_TO_BACK, BACK_TO_FRONT, TEXTURE
};
// stores the information needed to render a texture
struct Glyph {
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
	void begin(GlyphSortType sortType = GlyphSortType::TEXTURE);
	// closes and sorts the spritebatch
	void end();
	// adds a texture and its information to the spritebatch
	void draw(const glm::vec4& destRect, const glm::vec4& uvRect, GLuint texture, float depth, const Color& color);
	// renders the contents of the spritebatch to screen
	void renderBatch();
private: 
	void createRenderBatches();
	void createVertexArray();
	void sortGlyphs();

	static bool compareFrontToBack(Glyph* a, Glyph* b);
	static bool compareBackToFront(Glyph* a, Glyph* b);
	static bool compareTexture(Glyph* a, Glyph* b);

	GLuint _vboID;
	GLuint _vaoID;

	GlyphSortType _sortType;

	std::vector<Glyph*> _glyphs;
	std::vector<RenderBatch> _renderBatches;
};
}
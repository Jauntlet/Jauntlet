/* Purpose: Used to batch data for rendering on the GPU into one draw call instead of what would otherwise be many.
 * Main Contributer(s): Xander Mooney
 */
#pragma once

#include <GL/glew.h>
#include <glm/vec4.hpp>
#include <glm/vec2.hpp>
#include <vector>

#include "../Vertex.h"

// defines the order for sprites to be drawn
enum class GlyphSortType {
	NONE, FRONT_TO_BACK, BACK_TO_FRONT, TEXTURE
};
// stores the information needed to render a texture
class Glyph {
public:
	Glyph() : depth(0), texture(0) {};
	Glyph(const glm::vec4& DestRect, const glm::vec4& UvRect, unsigned int Texture, float Depth, const Color& Color);
	Glyph(const glm::vec4& DestRect, const glm::vec4& UvRect, float angle, unsigned int Texture, float Depth, const Color& Color);

	unsigned int texture;
	float depth;

	Vertex topLeft;
	Vertex bottomLeft;
	Vertex topRight;
	Vertex bottomRight;
private:
	// rotate point, extremely optimized just for this case.
	glm::vec2 rotatePoint(float x, float y, float angle);
};
// stores a texture, and information about rendering it
class RenderBatch {
public:
	RenderBatch(unsigned int Offset, unsigned int NumVertices, unsigned int Texture) : offset(Offset), numVertices(NumVertices), texture(Texture) {}
	unsigned int offset;
	unsigned int numVertices;
	unsigned int texture;
};
// combines information for loading textures into batches to send to the GPU at once.
class SpriteBatch
{
public:
	SpriteBatch();
	// Opens the spritebatch to be able to add textures to it
	void begin(GlyphSortType sortType = GlyphSortType::NONE);
	// adds a texture and its information to the spritebatch
	// destRect: where to draw the sprite X/Y = position, Z/W = width/height
	// uvRect: The UVs of the texture to be drawn
	// texture: The texture ID to draw
	// depth: the ordering in which to draw everything in the spriteBatch; only used if sorting was initialized in begin
	// color: color to draw the sprite in
	void draw(const glm::vec4& destRect, const glm::vec4& uvRect, unsigned int texture, float depth = 0, const Color& color = Color());
	// adds a texture and its information to the spritebatch;
	// destRect: where to draw the sprite X/Y = position, Z/W = width/height
	// texture: The texture ID to draw
	// depth: the ordering in which to draw everything in the spriteBatch; only used if sorting was initialized in begin
	// color: color to draw the sprite in
	void draw(const glm::vec4& destRect, unsigned int texture, float depth = 0, const Color& color = Color());
	// adds a texture and its information to the spritebatch
	// destRect: where to draw the sprite X/Y = position, Z/W = width/height
	// uvRect: The UVs of the texture to be drawn
	// degrees: direction to rotate the sprite
	// texture: The texture ID to draw
	// depth: the ordering in which to draw everything in the spriteBatch; only used if sorting was initialized in begin
	// color: color to draw the sprite in
	void draw(const glm::vec4& destRect, const glm::vec4& uvRect, float degrees, unsigned int texture, float depth = 0, const Color& color = Color());
	// adds a texture and its information to the spritebatch;
	// destRect: where to draw the sprite X/Y = position, Z/W = width/height
	// degrees: direction to rotate the sprite
	// texture: The texture ID to draw
	// depth: the ordering in which to draw everything in the spriteBatch; only used if sorting was initialized in begin
	// color: color to draw the sprite in
	void draw(const glm::vec4& destRect, float degrees, unsigned int texture, float depth = 0, const Color& color = Color());
	// adds a texture and its information to the spritebatch
	// destRect: where to draw the sprite X/Y = position, Z/W = width/height
	// uvRect: The UVs of the texture to be drawn
	// degrees: direction to rotate the sprite
	// texture: The texture ID to draw
	// depth: the ordering in which to draw everything in the spriteBatch; only used if sorting was initialized in begin
	// color: color to draw the sprite in
	void draw(const glm::vec4& destRect, const glm::vec4& uvRect, const glm::vec2& angle, unsigned int texture, float depth = 0, const Color& color = Color());
	// adds a texture and its information to the spritebatch;
	// destRect: where to draw the sprite X/Y = position, W/Z = width/height
	// degrees: direction to rotate the sprite
	// texture: The texture ID to draw
	// depth: the ordering in which to draw everything in the spriteBatch; only used if sorting was initialized in begin
	// color: color to draw the sprite in
	void draw(const glm::vec4& destRect, const glm::vec2& angle, unsigned int texture, float depth = 0, const Color& color = Color());

	void draw(Glyph glyph); // UNDOCUMENTED

	// closes and sorts the spritebatch
	void end();
	// renders the contents of the spritebatch to screen
	void render();
	// closes and renders the contents of the spritebatch to screen
	void endAndRender();
private: 
	void createRenderBatches();
	void createVertexArray();
	void sortGlyphs();

	static bool compareFrontToBack(Glyph* a, Glyph* b);
	static bool compareBackToFront(Glyph* a, Glyph* b);

	unsigned int _vboID = 0;
	unsigned int _vaoID = 0;

	GlyphSortType _sortType = GlyphSortType::NONE;

	std::vector<Glyph> _glyphs;
	// The reason why we have a seperate vector for pointing to the glyphs, is so that we can easily sort them to be rendered, without the negative performance
	// boost of having to allocate and unallocate memory
	std::vector<Glyph*> _glyphPointers;

	std::vector<RenderBatch> _renderBatches;
};
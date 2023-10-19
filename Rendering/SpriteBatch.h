/* Purpose: Used to batch data for rendering on the GPU into one draw call instead of what would otherwise be many.
 * Main Contributer(s): Xander Mooney
 */
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
	Glyph(const glm::vec4& DestRect, const glm::vec4& UvRect, GLuint Texture, float Depth, const Color& Color);
	Glyph(const glm::vec4& DestRect, const glm::vec4& UvRect, float angle, GLuint Texture, float Depth, const Color& Color);

	GLuint texture;
	float depth;

	Vertex topLeft;
	Vertex bottomLeft;
	Vertex topRight;
	Vertex bottomRight;
private:
	// rotate point, extremely optimized just for this case.
	glm::vec2 rotatePoint(const float& x, const float& y, const float& angle);
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
	// Opens the spritebatch to be able to add textures to it
	void begin(GlyphSortType sortType = GlyphSortType::NONE);
	// adds a texture and its information to the spritebatch
	// destRect: where to draw the sprite X/Y = position, Z/W = width/height
	// uvRect: The UVs of the texture to be drawn
	// texture: The texture ID to draw
	// depth: the ordering in which to draw everything in the spriteBatch; only used if sorting was initialized in begin
	// color: color to draw the sprite in
	void draw(const glm::vec4& destRect, const glm::vec4& uvRect, GLuint texture, float depth = 0, const Color& color = Color(255, 255, 255));
	// adds a texture and its information to the spritebatch;
	// destRect: where to draw the sprite X/Y = position, Z/W = width/height
	// texture: The texture ID to draw
	// depth: the ordering in which to draw everything in the spriteBatch; only used if sorting was initialized in begin
	// color: color to draw the sprite in
	void draw(const glm::vec4& destRect, GLuint texture, float depth = 0, const Color& color = Color(255, 255, 255));
	// adds a texture and its information to the spritebatch
	// destRect: where to draw the sprite X/Y = position, Z/W = width/height
	// uvRect: The UVs of the texture to be drawn
	// degrees: direction to rotate the sprite
	// texture: The texture ID to draw
	// depth: the ordering in which to draw everything in the spriteBatch; only used if sorting was initialized in begin
	// color: color to draw the sprite in
	void draw(const glm::vec4& destRect, const glm::vec4& uvRect, float degrees, GLuint texture, float depth = 0, const Color& color = Color(255, 255, 255));
	// adds a texture and its information to the spritebatch;
	// destRect: where to draw the sprite X/Y = position, Z/W = width/height
	// degrees: direction to rotate the sprite
	// texture: The texture ID to draw
	// depth: the ordering in which to draw everything in the spriteBatch; only used if sorting was initialized in begin
	// color: color to draw the sprite in
	void draw(const glm::vec4& destRect, float degrees, GLuint texture, float depth = 0, const Color& color = Color(255, 255, 255));
	// adds a texture and its information to the spritebatch
	// destRect: where to draw the sprite X/Y = position, Z/W = width/height
	// uvRect: The UVs of the texture to be drawn
	// degrees: direction to rotate the sprite
	// texture: The texture ID to draw
	// depth: the ordering in which to draw everything in the spriteBatch; only used if sorting was initialized in begin
	// color: color to draw the sprite in
	void draw(const glm::vec4& destRect, const glm::vec4& uvRect, const glm::vec2& angle, GLuint texture, float depth = 0, const Color& color = Color(255, 255, 255));
	// adds a texture and its information to the spritebatch;
	// destRect: where to draw the sprite X/Y = position, W/Z = width/height
	// degrees: direction to rotate the sprite
	// texture: The texture ID to draw
	// depth: the ordering in which to draw everything in the spriteBatch; only used if sorting was initialized in begin
	// color: color to draw the sprite in
	void draw(const glm::vec4& destRect, const glm::vec2& angle, GLuint texture, float depth = 0, const Color& color = Color(255, 255, 255));

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
/* Purpose: Holds information for a texture to be rendered onscreen
 * Main Contributer(s): Xander Mooney
 */
#pragma once

#include <GL/glew.h>

namespace Jauntlet {
// struct for Textures
struct GLTexture {
	GLuint id;
	int width;
	int height;
};
}
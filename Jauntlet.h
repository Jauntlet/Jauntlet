/* Purpose: Used to initialize systems for Jauntlet to function and properly log errors.
 * Main Contributer(s): Xander Mooney
 */
#pragma once

#ifdef VULKAN
#else
#define OPENGL
#include <GL/glew.h>
#endif

namespace Jauntlet {
// initializes the game engine
extern int init();
}
/* Purpose: An individual thing made by a particle
 * Main Contributer(s): Jack Kennedy
 */

#include <glm/glm.hpp>
#include <vector>

#include "../SpriteBatch.h" // Jauntlet::SpriteBatch
#include "../Cameras/Camera2D.h" // Jauntlet::Camera2D
#include "../Vertex.h" // Jauntlet::Color

#pragma once

namespace Jauntlet {
class Emission
{
public:
    Emission();

    void draw(SpriteBatch* spriteBatch, Camera2D* camera); // DONTDOCUMENT

    // our current position
    glm::vec2 position;
    // our current size
    glm::vec2 size;
    // the movement we make in one second
    glm::vec2 movement;
    // the current color of our emission
    Color color;
    // the life of our emission
    float time;
private:
};
}
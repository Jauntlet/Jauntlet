/* Purpose: An individual thing made by a particle
 * Main Contributer(s): Jack Kennedy
 */

#include <glm/ext/vector_float2.hpp>
#include <vector>

#include "../Textures/SpriteBatch.h" // Jauntlet::SpriteBatch
#include "../Cameras/Camera2D.h" // Jauntlet::Camera2DImages/

#pragma once

namespace Jauntlet {
class Emission
{
public:
    Emission();

    // draw the emission
    void draw(SpriteBatch* spriteBatch, Camera2D* camera, unsigned int texture); // DONTDOCUMENT

    // update the emission
    void update();

    // our current position
    glm::vec2 position = glm::vec2(0);
    // our current size
    glm::vec2 size = glm::vec2(10);
    // the movement we make in one second
    glm::vec2 movement = glm::vec2(0,10);
    // the current color of our emission
    Color color;
    // the age of our emission
    float age = 0.0f;
    // the life expectancy of our emission
    float lifetime = 10.0f;
private:
};
}
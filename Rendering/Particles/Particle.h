/* Purpose: Sets up and plays particle effect animations.
 * Main Contributer(s): Jack Kennedy
 */

#include "../../Time.h"
#include "../SpriteBatch.h"
#include "Properties/ParticleProperty.h"

#include "Emission.h"

#include <string>
#include <vector>

#pragma once

namespace Jauntlet {
class Particle
{
public:
    Particle(
        const ParticleProperty properties[],
        Camera2D* camera,
        glm::vec2* position,
        // the properties
        std::string texture,
        // pre determined
        uint8_t maxAliveMembers = 10,
        bool emitContinuously = true,
        float emissionSpeed = 10,
        Color startingColor = Color()
        
        ); // UNDOCUMENTED

    // draw the emissions
    void draw(); // UNDOCUMENTED

    // update emissions (positions, size, etc)
    void update();

    std::vector<Emission> emissions; // DONTDOCUMENT
private:
    SpriteBatch _spriteBatch;
    std::vector<ParticleProperty> _properties;
    
    Camera2D* _camera;
    glm::vec2* _position;

    GLuint _texture;

    uint8_t _maxEmissions = 0;
};
}
/* Purpose: Sets up and plays particle effect animations.
 * Main Contributer(s): Jack Kennedy
 */

#include "Properties/ParticleGrow.h"

#include <string>
#include <vector>

#pragma once

namespace Jauntlet {
class Particle
{
public:
    Particle(
        Camera2D* camera,
        glm::vec2 position,
        // the properties
        std::string texture,
        // pre determined
        uint8_t maxAliveMembers = 10,
        bool emitContinuously = true,
        float emissionSpeed = 1,
        Color startingColor = Color()
        
        ); // UNDOCUMENTED

    // draw the emissions
    void draw(); // UNDOCUMENTED

    // update emissions (positions, size, etc)
    void update();

    void addProperty(ParticleProperty& property);

    std::vector<Emission> emissions; // DONTDOCUMENT
private:
    SpriteBatch _spriteBatch;
    std::vector<ParticleProperty*> _properties;

    Camera2D* _camera;
    glm::vec2 _position;

    GLuint _texture;

    uint8_t _maxEmissions = 0;
    uint8_t _emissionsCount = 0;

    float _timer = 0;

    float _emissionSpeed;

    void emit();
};
}
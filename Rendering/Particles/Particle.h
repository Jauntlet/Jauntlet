/* Purpose: Sets up and plays particle effect animations.
 * Main Contributer(s): Jack Kennedy
 */

#include "../../Camera2D.h"
#include "../../Time.h"
#include "../SpriteBatch.h"
#include "Properties/ParticleProperty.h"

#include "Emission.h"

#include <vector>

#pragma once

namespace Jauntlet {
class Particle
{
public:
    Particle(ParticleProperty properties[]); // UNDOCUMENTED

    void draw(Camera2D* camera2D); // UNDOCUMENTED

    std::vector<Emission> emissions; // DONTDOCUMENT
private:
    SpriteBatch _spriteBatch;
    std::vector<ParticleProperty> _properties;

    SpriteBatch* _spriteBatchPtr;
    Camera2D _camera;
};
}
#include "Particle.h"
#include "Emission.h"

#include "../ResourceManager.h"

using namespace Jauntlet;

Particle::Particle(
        const ParticleProperty properties[],
        Camera2D* camera,
        glm::vec2* position,
        // the properties
        std::string texture,
        // pre determined
        uint8_t maxAliveMembers,
        bool emitContinuously,
        float emissionSpeed,
        Color startingColor
        
        ) {
    _camera = camera;
    _position = position;
    _texture = Jauntlet::ResourceManager::getTexture(texture).id;

    for (int i = 0; i < maxAliveMembers; ++i) {
        emissions.push_back(Emission());
    }
}

void Particle::draw() {
    _spriteBatch.begin();

    for (int i = 0; i < emissions.size(); ++i) {
        emissions[i].draw(&_spriteBatch,_camera,_texture);
    }

    _spriteBatch.endAndRender();
}
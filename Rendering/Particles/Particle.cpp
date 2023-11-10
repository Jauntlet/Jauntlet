#include "Particle.h"
#include "Emission.h"
#include "../../Time.h"

#include "../ResourceManager.h"
#include "Properties/ParticleGrow.h"
#include "Properties/ParticleProperty.h"

#include <iostream>

using namespace Jauntlet;

Particle::Particle(
        Camera2D* camera,
        glm::vec2 position,
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
    _emissionSpeed = emissionSpeed;
    _maxEmissions = maxAliveMembers;
}

void Particle::draw() {
    _spriteBatch.begin();

   for (int i = 0; i < emissions.size(); ++i) {
        emissions[i].draw(&_spriteBatch,_camera,_texture);
    }

    _spriteBatch.endAndRender();
}

void Particle::update() {
    _timer += Time::getDeltaTime();
    
    while (_emissionsCount < _maxEmissions && _timer > _emissionSpeed) {
        emit();
        _timer -= _emissionSpeed;
    }

    for (int i = 0; i < emissions.size(); ++i) {
        emissions[i].update();

        if (emissions[i].age > emissions[i].lifetime) {
            emissions.erase(emissions.begin() + i);
            --i;
            --_emissionsCount;
        }
    }

    ParticleGrow bruh = ParticleGrow(0,100);
    bruh.apply(emissions);
}

void Particle::emit() {
    // there isnt really a care where we want to add an emission and not increment this counter
    ++_emissionsCount;
    emissions.push_back(Emission());
}

void Particle::addProperty(ParticleProperty& property) {
    _properties.push_back(&property);
}
#include "Particle.h"

#include "../Textures/ResourceManager.h"

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
        if (_doEmit) {
            emit();
        }
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
    // there isnt really a case where we want to add an emission and not increment this counter
    ++_emissionsCount;

    Emission emission = Emission();
    emission.position = _position;
    emissions.push_back(emission);
}

void Particle::addProperty(ParticleProperty& property) {
    _properties.push_back(&property);
}

void Particle::resumeEmitting() {
    _doEmit = true;
}

void Particle::stopEmitting() {
    _doEmit = false;
}
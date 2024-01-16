#include "ParticleEmitter.h"

#include "../Textures/ResourceManager.h"

ParticleEmitter::ParticleEmitter(
        // the properties
        Camera2D* camera,
        glm::vec2 position,
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

void ParticleEmitter::draw() {
    _spriteBatch.begin();

   for (size_t i = 0; i < emissions.size(); ++i) {
        emissions[i].draw(&_spriteBatch,_camera,_texture);
    }

    _spriteBatch.endAndRender();
}

void ParticleEmitter::update() {
    _timer += Jauntlet::Time::getDeltaTime();
    
    while (_emissionsCount < _maxEmissions && _timer > _emissionSpeed) {
        if (_doEmit) {
            emit();
        }
        _timer -= _emissionSpeed;
    }

    for (size_t i = 0; i < emissions.size(); ++i) {
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

void ParticleEmitter::emit() {
    // there isnt really a case where we want to add an emission and not increment this counter
    ++_emissionsCount;

    Particle particle = Particle();
    particle.position = _position;
    emissions.push_back(particle);
}

void ParticleEmitter::addProperty(ParticleProperty& property) {
    _properties.push_back(&property);
}

void ParticleEmitter::resumeEmitting() {
    _doEmit = true;
}

void ParticleEmitter::stopEmitting() {
    _doEmit = false;
}
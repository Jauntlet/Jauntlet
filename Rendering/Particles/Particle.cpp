#include "Particle.h"

using namespace Jauntlet;

Particle::Particle(ParticleProperty properties[], Camera2D* camera, glm::vec2* position) {
    _camera = camera;
    _position = position;
}

void Particle::draw(Camera2D* camera) {
    _spriteBatch.begin();

    for (int i = 0; i < emissions.size(); ++i) {
        emissions[i].draw(&_spriteBatch,_camera);
    }

    _spriteBatch.endAndRender();
}
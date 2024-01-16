#include "Particle.h"

Particle::Particle() {
    
}

void Particle::draw(SpriteBatch* spriteBatch, Camera2D* camera, unsigned int texture) {
    spriteBatch->draw(glm::vec4(position.x - size.x * 0.5f,position.y  - size.y * 0.5f,size.x,size.y), texture, 0, Color(255,255,255,255));
}

void Particle::update() {
    position += movement * Jauntlet::Time::getDeltaTime();
    age += Jauntlet::Time::getDeltaTime();
}
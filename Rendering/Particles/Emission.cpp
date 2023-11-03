#include "Emission.h"

#include "../../Time.h"

using namespace Jauntlet;

Emission::Emission() {
    
}

void Emission::draw(SpriteBatch* spriteBatch, Camera2D* camera, GLuint texture) {
    spriteBatch->draw(glm::vec4(0,0,10,10), glm::vec4(0,0,1,1), texture, 0, Jauntlet::Color());//, //);
}

void Emission::update() {
    position += movement * Time::getDeltaTime();
}
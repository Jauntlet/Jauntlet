#include "Player.h"
#include<Jauntlet/ResourceManager.h>
#include<Jauntlet/Vertex.h>
#include<SDL/SDL.h>

Player::Player(float x, float y) {
	_position = glm::vec2(x, y);
}

void Player::update(Jauntlet::InputManager& inputManager) {
	const float SPEED = 3;
	
	if (inputManager.isKeyDown(SDLK_w)) {
		_position.y += SPEED;
	}
	if (inputManager.isKeyDown(SDLK_s)) {
		_position.y -= SPEED;
	}
	if (inputManager.isKeyDown(SDLK_a)) {
		_position.x -= SPEED;
	}
	if (inputManager.isKeyDown(SDLK_d)) {
		_position.x += SPEED;
	}
}

void Player::draw(Jauntlet::SpriteBatch& spriteBatch) {
	// For some reason I am getting errors using ResourceManager currently. WILL BE LOOKED INTO SOON.
	static int textureID = 0; //Jauntlet::ResourceManager::getTexture("Textures/Craig.png").id;

	const glm::vec4 uvRect(0, 0, 1, 1);
	glm::vec4 destRect;
	// position to draw player
	destRect.x = _position.x;
	destRect.y = _position.y;
	// width / height of player
	destRect.z = 32;
	destRect.w = 32;

	static Jauntlet::Color color;
	color.setColor(255, 255, 255, 255);

	spriteBatch.draw(destRect, uvRect, textureID, 0, color);
}

void Player::setPosition(float x, float y) {
	_position = glm::vec2(x, y);
}
void Player::setPosition(glm::vec2 pos) {
	_position = pos;
}
glm::vec2 Player::getPosition() {
	return _position;
}
#include "Player.h"
#include<Jauntlet/ResourceManager.h>
#include<Jauntlet/Vertex.h>
#include<SDL/SDL.h>
#include<Jauntlet/Time.h>

Player::Player(float x, float y) {
	_position = glm::vec2(x, y);
}

void Player::update(Jauntlet::InputManager& inputManager) {
	const float SPEED = 120;
	
	glm::vec2 velocity(0, 0);

	if (inputManager.isKeyDown(SDLK_w)) {
		velocity.y += 1; //_speed * Jauntlet::Time::getDeltaTime();
	}
	if (inputManager.isKeyDown(SDLK_s)) {
		velocity.y -= 1; //_speed * Jauntlet::Time::getDeltaTime();
	}
	if (inputManager.isKeyDown(SDLK_a)) {
		velocity.x -= 1; //_speed * Jauntlet::Time::getDeltaTime();
	}
	if (inputManager.isKeyDown(SDLK_d)) {
		velocity.x += 1; //_speed * Jauntlet::Time::getDeltaTime();
	}
	
	// for some reason position breaks if it runs while velocity is 0????
	if (velocity != glm::vec2(0, 0)) {
		_position += glm::normalize(velocity) * (_speed * Jauntlet::Time::getDeltaTime());
	}
}

void Player::draw(Jauntlet::SpriteBatch& spriteBatch) {
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

	spriteBatch.draw(destRect, uvRect, Jauntlet::ResourceManager::getTexture("Textures/Craig.png").id, 0, color);
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
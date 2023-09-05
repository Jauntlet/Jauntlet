#include "Player.h"
#include <Jauntlet/Rendering/ResourceManager.h>
#include <Jauntlet/Rendering/Vertex.h>
#include <SDL/SDL.h>
#include <Jauntlet/Time.h>

Player::Player(float x, float y, Jauntlet::InputManager* inputManager) : _inputManager(inputManager), _moveUp(_inputManager), _moveLeft(_inputManager),
																	     _moveRight(_inputManager), _moveDown(_inputManager), collider(CircleCollider2D(16.0f, _position)) {
	_position = glm::vec2(x, y);

	_moveUp.addKey(SDLK_w);
	_moveUp.addKey(SDLK_UP);

	_moveLeft.addKey(SDLK_a);
	_moveLeft.addKey(SDLK_LEFT);

	_moveRight.addKey(SDLK_d);
	_moveRight.addKey(SDLK_RIGHT);

	_moveDown.addKey(SDLK_s);
	_moveDown.addKey(SDLK_DOWN);
}

void Player::update() {
	
	glm::vec2 velocity(0, 0);

	if (_moveUp.isDown()) {
		velocity.y += 1;
	}
	if (_moveDown.isDown()) {
		velocity.y -= 1;
	}
	if (_moveLeft.isDown()) {
		velocity.x -= 1;
	}
	if (_moveRight.isDown()) {
		velocity.x += 1;
	}

	// for some reason position breaks if it runs while velocity is 0????
	if (velocity != glm::vec2(0, 0)) {
		_position += glm::normalize(velocity) * (_speed * Jauntlet::Time::getDeltaTime());
	}

	//update collider
	collider.position = _position;
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
	collider.position = pos;
}

void Player::setSpeed(float newSpeed) {
	_speed = newSpeed;
}
float Player::getSpeed() {
	return _speed;
}

glm::vec2 Player::getPosition() {
	return _position;
}
#pragma once

#include <Jauntlet/Collision/Collision2D.h>
#include <Jauntlet/InputKey.h>
#include <Jauntlet/InputManager.h>
#include <Jauntlet/Rendering/SpriteBatch.h>

class Player
{
public:
	Player(float x, float y, Jauntlet::InputManager* inputManager);

	void update();

	void draw(Jauntlet::SpriteBatch& spriteBatch);

	void setPosition(float x, float y);
	void setPosition(glm::vec2 pos);

	void setSpeed(float newSpeed);
	float getSpeed();

	glm::vec2 getPosition();

	Jauntlet::CircleCollider2D collider;
private:
	glm::vec2 _position = glm::vec2(0,0);

	float _speed = 120;

	Jauntlet::InputManager* _inputManager;

	Jauntlet::InputKey _moveUp;
	Jauntlet::InputKey _moveLeft;
	Jauntlet::InputKey _moveRight;
	Jauntlet::InputKey _moveDown;
};
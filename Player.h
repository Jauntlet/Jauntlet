#pragma once
#include<Jauntlet/Rendering/SpriteBatch.h>
#include<Jauntlet/InputManager.h>
#include<Jauntlet/InputKey.h>
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
private:
	glm::vec2 _position;

	float _speed;

	Jauntlet::InputManager* _inputManager;

	Jauntlet::InputKey _moveUp;
	Jauntlet::InputKey _moveLeft;
	Jauntlet::InputKey _moveRight;
	Jauntlet::InputKey _moveDown;
};
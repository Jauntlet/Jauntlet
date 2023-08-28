#pragma once
#include<Jauntlet/Rendering/SpriteBatch.h>
#include<Jauntlet/InputManager.h>
class Player
{
public:
	Player(float x, float y);

	void update(Jauntlet::InputManager& inputManager);

	void draw(Jauntlet::SpriteBatch& spriteBatch);

	void setPosition(float x, float y);
	void setPosition(glm::vec2 pos);

	void setSpeed(float newSpeed);
	float getSpeed();

	glm::vec2 getPosition();
private:
	glm::vec2 _position;

	float _speed;
};
#pragma once

#include <glm/fwd.hpp>
#include <glm/glm.hpp>
#include <string>

#include "../Time.h"

namespace Jauntlet {
class Animation
{
public:
	Animation(glm::vec2 spriteSize, glm::vec2 animationSize);
	void play(int start, int end, float frameTime);
	void update();
	void stop();
	glm::vec4 getUV();
private:

	float _elapsedTime = 0;
	
	glm::vec2 _spriteSize;

	int _start;
	int _end;

	int _frame;

	float _frameTime;

	bool _playing = false;

	glm::vec4 _uv = glm::vec4(0,0,1,1);
};
}
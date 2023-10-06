#pragma once

#include <glm/fwd.hpp>
#include <glm/glm.hpp>
#include <string>

#include "../Time.h"

namespace Jauntlet {
class Animation
{
public:
	Animation();
	Animation(glm::vec2 spriteSize, glm::vec2 animationSize);
	Animation(int frames);
	void play(int start, int end, float frameTime);
	void update();
	void stop();
	void pause();
	glm::vec4 getUV();
private:

	float _elapsedTime = 0;
	
	float _spriteSize;

	int _start;
	int _end;

	int _frame = -1;

	float _frameTime;

	bool _playing = false;

	glm::vec4 _uv = glm::vec4(0,0,1,1);
};
}
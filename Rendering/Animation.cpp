#include "Animation.h"

using namespace Jauntlet;

Animation::Animation(glm::vec2 spriteSize, glm::vec2 animationSize) {
	_spriteSize = spriteSize / animationSize;
}

void Animation::play(int start, int end, float frameTime) {
	_elapsedTime = 0.0f;
	_start = start;
	_end = end;
	_frameTime = frameTime;
	_playing = true;
	_frame = start;
}

void Animation::stop() {
	_playing = false;
}

void Animation::update() {
	if (!_playing) {
		return;
	}

	// since this is run every frame, we'll do this.
	_elapsedTime += Time::getDeltaTime();
	
	while (_elapsedTime > _frameTime) {
		// increment by one unless above end, if so set to start.
		_frame = ++_frame > _end ? _start : _frame;
		_elapsedTime -= _frameTime;
	}

	_uv = glm::vec4(_spriteSize.x * _frame,0,_spriteSize.x * (_frame - 1),1);
}

glm::vec4 Animation::getUV() {
	return _uv;
}
#include "AnimationFrame.h"
#include "../time.h"
#include "Time.h"
#include "glm/fwd.hpp"

using namespace Jauntlet;

static int _lastestID = 0;

AnimationFrame::AnimationFrame(std::string filePath, glm::vec2 spriteSize) {
	_spriteSize = spriteSize;
}

void AnimationFrame::play(int start, int end, float frameTime) {
	_elapsedTime = 0;
	_start = start;
	_end = end;
	_frameTime = frameTime;
	_playing = true;
	_frame = start;
}

void AnimationFrame::stop() {
	_playing = false;
}

void AnimationFrame::update(float deltaTime) {
	if (!_playing) {
		return;
	}

	_elapsedTime += deltaTime;
	
	while (_elapsedTime > _frameTime) {
		// increment by one unless above end, if so set to start.
		_frame = ++_frame > _end ? _start : _frame;
		_elapsedTime -= _frameTime;
	}

	_uv = glm::vec4(_spriteSize.x * _frame,0,_spriteSize.x * (_frame - 1),_spriteSize.y);
}

glm::vec4 AnimationFrame::getUV() {
	return _uv;
}
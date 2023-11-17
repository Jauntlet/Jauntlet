#include "Animation.h"

#include "../../Time.h"

using namespace Jauntlet;

Animation::Animation(int frames) {
	_spriteSize = 1.0f / frames;
}

void Animation::play(int start, int end, float frameTime, bool loop) {
	_elapsedTime = 0.0f;
	_start = start;
	_end = end;
	// absolute value so that if negative is input we don't crash the game
	_frameTime = std::abs(frameTime);
	_playing = true;
	_loop = loop;

	_frame = start;
}

void Animation::pause() {
	_playing = false;
}

void Animation::resume() {
	_playing = true;
}

void Animation::stop() {
	_playing = false;
	_frame = _start;
}
void Animation::stop(int frame) {
	_playing = false;
	_frame = frame;
	_uv = glm::vec4(_spriteSize * _frame, 0, _spriteSize, 1);
}

bool Animation::update() {
	if (!_playing) {
		return false;
	}

	_elapsedTime += Time::getDeltaTime();
	
	while (_elapsedTime > _frameTime) {
		++_frame;
		if (_frame > _end) {
			if (_loop) {
				_frame = _start;
			}
			else {
				stop();
				return false;
			}
		}
		_elapsedTime -= _frameTime;
	}

	_uv = glm::vec4(_spriteSize * _frame,0,_spriteSize,1);
	return true;
}

glm::vec4 Animation::getUV() {
	return _uv;
}
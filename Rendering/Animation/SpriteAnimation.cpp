#include "SpriteAnimation.h"

#include "../../Time.h"

SpriteAnimation::SpriteAnimation(unsigned int frames) {
	_spriteSize = 1.0f / frames;
}

void SpriteAnimation::play(int start, int end, float frameTime, bool loop) {
	_elapsedTime = 0.0f;
	_start = start;
	_end = end;
	// absolute value so that if negative is input we don't crash the game
	_frameTime = std::abs(frameTime);
	_playing = true;
	_loop = loop;

	_frame = start;
}

void SpriteAnimation::pause() {
	_playing = false;
}
void SpriteAnimation::pause(int frame) {
	pause();
	_frame = frame;
	recalculateUV();
}

void SpriteAnimation::resume() {
	_playing = true;
}

void SpriteAnimation::stop() {
	_playing = false;
	_frame = _start;
}
void SpriteAnimation::stop(int frame) {
	_playing = false;
	_frame = frame;
	recalculateUV();
}
bool SpriteAnimation::update() {
	if (!_playing) {
		return false;
	}

	_elapsedTime += Jauntlet::Time::getDeltaTime();
	
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
		recalculateUV();
	}


	return true;
}

glm::vec4 SpriteAnimation::getUV() const {
	return _uv;
}

int SpriteAnimation::getCurrentFrame() const {
	return _frame;
}

void SpriteAnimation::recalculateUV() {
	_uv = glm::vec4(_spriteSize * _frame, 0, _spriteSize, 1);
}
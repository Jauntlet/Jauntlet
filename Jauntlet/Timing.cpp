#pragma once
#include"Timing.h"

#include<SDL/SDL.h>

namespace Jauntlet
{
FpsLimiter::FpsLimiter() {

}
void FpsLimiter::init(int maxFPS) {
	setMaxFPS(maxFPS);
}

void FpsLimiter::setMaxFPS(int maxFPS) {
	_maxFPS = maxFPS;
}

void FpsLimiter::beginFrame() {
	_startTicks = SDL_GetTicks();
}

int FpsLimiter::endFrame() {
	calculateFPS();

	// FPS limiting 
	float frameTicks = SDL_GetTicks() - _startTicks;
	if (1000 / _maxFPS > frameTicks) {
		SDL_Delay(1000 / _maxFPS - frameTicks);
	}

	return _fps;
}

void FpsLimiter::calculateFPS() {
	// NUM_SAMPLES is how many frames we are averaging by
	static const int NUM_SAMPLES = 10;
	static float frameTimes[NUM_SAMPLES] = {};
	static int currentFrame = 0;
	static float prevTicks = SDL_GetTicks();

	float currentTicks = SDL_GetTicks();

	_frameTime = currentTicks - prevTicks;
	frameTimes[currentFrame % NUM_SAMPLES] = _frameTime;

	prevTicks = currentTicks;

	int count = NUM_SAMPLES;

	currentFrame++;

	if (currentFrame < NUM_SAMPLES) {
		count = currentFrame;
	}

	float frameTimeAverage = 0;
	for (int i = 0; i < count; i++) {
		frameTimeAverage += frameTimes[i];
	}
	frameTimeAverage /= count;

	if (frameTimeAverage > 0) {
		_fps = 1000 / frameTimeAverage;
	}
	else {
		_fps = 0;
	}
}
}
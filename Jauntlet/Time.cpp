#pragma once
#include"Time.h"

#include<SDL/SDL.h>

namespace Jauntlet
{
// all of the initialized variables are instantly overwritten, so don't worry about them (its to remove warnings)
Time::Time() : _fps(0), _deltaTime(0), _maxFPS(120), _startTicks(0) {

}
void Time::init(int maxFPS) {
	setMaxFPS(maxFPS);
}

void Time::setMaxFPS(int maxFPS) {
	_maxFPS = maxFPS;
}

void Time::beginFrame() {
	_startTicks = SDL_GetTicks();
}

float Time::endFrame() {
	calculateFPS();

	// FPS limiting 
	float frameTicks = SDL_GetTicks() - _startTicks;
	if (1000 / _maxFPS > frameTicks) {
		SDL_Delay(1000 / _maxFPS - frameTicks);
	}

	return _fps;
}

 float Time::getDeltaTime() {
	return _deltaTime/1000;
}
float Time::getTimeScale() {
	return _timeScale;
}
void Time::setTimeScale(float newScale) {
	_timeScale = newScale;
}
void Time::calculateFPS() {
	// NUM_SAMPLES is how many frames we are averaging by
	static const int NUM_SAMPLES = 10;
	static float frameTimes[NUM_SAMPLES] = {};
	static int currentFrame = 0;
	static float prevTicks = SDL_GetTicks();

	float currentTicks = SDL_GetTicks();

	_deltaTime = currentTicks - prevTicks;
	frameTimes[currentFrame % NUM_SAMPLES] = _deltaTime;
	
	prevTicks = currentTicks;


	currentFrame++;

	float frameTimeAverage = 0;
	for (int i = 0; i < NUM_SAMPLES; i++) {
		frameTimeAverage += frameTimes[i];
	}
	frameTimeAverage /= NUM_SAMPLES;

	if (frameTimeAverage > 0) {
		_fps = 1000 / frameTimeAverage;
	}
	else {
		_fps = 0;
	}
}
}
#include <SDL/SDL.h>

#include "Time.h"

using namespace Jauntlet;

float Time::_fps = 0;
float Time::_maxFPS = -1;
float Time::_deltaTime = 0;
float Time::_timeScale = 1;
long Time::_time = 0;
unsigned int Time::_startTicks;

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

long Time::getTime() {
	return _time;
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

	_deltaTime = (currentTicks - prevTicks) * _timeScale;
	frameTimes[currentFrame % NUM_SAMPLES] = _deltaTime;
	
	prevTicks = currentTicks;


	currentFrame++;

	float frameTimeAverage = 0;
	for (int i = 0; i < NUM_SAMPLES; ++i) {
		frameTimeAverage += frameTimes[i];
	}
	frameTimeAverage /= NUM_SAMPLES;

	if (frameTimeAverage > 0) {
		_fps = 1000 / frameTimeAverage;
	}
	else {
		_fps = 0;
	}
	_time += _deltaTime;
}
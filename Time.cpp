#include "Time.h"
#include <SDL2/SDL_timer.h>
#include "Jauntlet.h"

using namespace Jauntlet;

float Time::_fps = 0;
int Time::_maxFPS = -1;
float Time::_deltaTime = 0;
float Time::_timeScale = 1;
double Time::_time = 0;
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
	float frameTicks = static_cast<float>(SDL_GetTicks()) - _startTicks;
	if (1000 / _maxFPS > frameTicks) {
		SDL_Delay(1000 / _maxFPS - static_cast<float>(frameTicks));
	}

	return _fps;
}

 float Time::getDeltaTime() {
	return _deltaTime;
}
float Time::getTimeScale() {
	return _timeScale;
}

double Time::getTime() {
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
	
	// remember that this works becasue the static "prevTicks" only gets initalized once, meaning that we are not actually setting it to 
	// SDL_GetTicks() every time this function is called.  -xm

	// I'd also like to look into using SDL_GetTicks64() on x64 versions of Jauntlet, however I have found it difficult to reliably figure out if 
	// the game is x86 or x64 consistently across platforms. Initial looks have found `__SIZEOF_POINTER__ == 4` to be a possible way, 
	// but it doesn't seem to work consistently as I'd like it to.
	static Uint32 prevTicks = SDL_GetTicks();
	Uint32 currentTicks = SDL_GetTicks();

	_deltaTime = (currentTicks - prevTicks) * _timeScale;
	frameTimes[currentFrame % NUM_SAMPLES] = _deltaTime;
	_deltaTime /= 1000;

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
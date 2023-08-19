#pragma once
namespace Jauntlet{
class Time {
public:
	//Time class constructor
	Time();
	//initialize fps
	void init(int maxFPS);
	void setMaxFPS(int maxFPS);

	void beginFrame();
	// returns current FPS
	float endFrame();
	//returns time from last frame in seconds
	float getDeltaTime();
// returns how fast the game is running (1 is default)
   float getTimeScale();
   //get game speed
	void setTimeScale(float newScale);


private:
	void calculateFPS();

	float _fps;
	float _maxFPS;
	float _deltaTime;
	float _timeScale;
	unsigned int _startTicks;
};
}
#pragma once
namespace Jauntlet{
class Time {
public:
	//initialize fps
	static void setMaxFPS(int maxFPS);

	static void beginFrame();
	// returns current FPS
	static float endFrame();
	//returns time from last frame in seconds
	static float getDeltaTime();
	// returns how fast the game is running (1 is default)
    static float getTimeScale();
    //get game speed
	static void setTimeScale(float newScale);

private:
	static void calculateFPS();

	static float _fps;
	static float _maxFPS;
	static float _deltaTime;
	static float _timeScale;
	static unsigned int _startTicks;
};
}
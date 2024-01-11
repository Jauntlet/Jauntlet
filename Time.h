/* Purpose: Handles time globally within Jauntlet
 * Main Contributer(s): Xander Mooney
 */
#pragma once

namespace Jauntlet{
class Time {
public:
	//initialize fps
	static void setMaxFPS(int maxFPS);
	// put at the start of a frame to find the length of a frame
	static void beginFrame();
	// put at the end of a frame to calculate and return current FPS
	static float endFrame();
	//returns time from last frame in seconds
	static float getDeltaTime();
	// returns how fast the game is running (1 is default)
    static float getTimeScale();
    //get game speed
	static void setTimeScale(float newScale);
	//get time elapsed since game start
	static double getTime();

private:
	static void calculateFPS();

	static float _fps;
	static int _maxFPS;
	static float _deltaTime;
	static float _timeScale;
	static double _time;
	static unsigned int _startTicks;
};
}
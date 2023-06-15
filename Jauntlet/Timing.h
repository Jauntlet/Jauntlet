#pragma once
namespace Jauntlet{
class FpsLimiter {
public:
	FpsLimiter();
	void init(int maxFPS);
	void setMaxFPS(int maxFPS);

	void beginFrame();
	// returns current FPS
	int endFrame();
private:
	void calculateFPS();

	float _fps;
	float _frameTime;
	int _maxFPS;
	unsigned int _startTicks;
};
}
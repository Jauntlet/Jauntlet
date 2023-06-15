#pragma once
namespace Jauntlet{
class FpsLimiter {
public:
	FpsLimiter();
	void init(int maxFPS);
	void setMaxFPS(int maxFPS);

	void beginFrame();
	// returns current FPS
	float endFrame();
private:
	void calculateFPS();

	float _fps;
	float _frameTime;
	float _maxFPS;
	unsigned int _startTicks;
};
}
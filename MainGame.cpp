#include "MainGame.h"
#include<iostream>
#include<string>
#include<Jauntlet/Errors.h>

MainGame::MainGame() : _screenWidth(1024), _screenHeight(768), _gameState(GameState::PLAY), time(0), _maxFPS(120), _fps(0), _frameTime(0), _window() {

}

void MainGame::run() {
	initSystems();

	// debugging code: to be removed
	_sprite.init(-1, -1, 2, 2, "Textures/Craig.png");

	gameLoop();
}

void MainGame::initSystems() {

	Jauntlet::init();

	_window.create("Jauntlet Game Engine", _screenWidth, _screenHeight, 0);

	initShaders();
}

void MainGame::initShaders() {
	_colorProgram.compileShaders("Shaders/colorShading.vert", "Shaders/colorShading.frag");
	_colorProgram.addAttribute("vertexPosition");
	_colorProgram.addAttribute("vertexColor");
	_colorProgram.addAttribute("vertexUV");
	_colorProgram.linkShaders();
}

void MainGame::gameLoop() {
	while (_gameState != GameState::EXIT) {
		float startTicks = SDL_GetTicks();
		
		processInput();

		// temporary time
		time += 0.01;

		drawGame();
		calculateFPS();
		
		// debug output FPS every 10 frames
		static int frameCounter = 0;
		frameCounter++;
		if (frameCounter == 10) {
			std::cout << _fps << std::endl;
			frameCounter = 0;
		}

		// FPS limiting 
		float frameTicks = SDL_GetTicks() - startTicks;
		if (1000 / _maxFPS > frameTicks) {
			SDL_Delay(1000 / _maxFPS - frameTicks);
		}
	}
}

void MainGame::processInput() {
	SDL_Event evnt;
	
	while (SDL_PollEvent(&evnt))
	{
		switch (evnt.type)
		{
			case SDL_QUIT:
				_gameState = GameState::EXIT;
				break;
		}
	}
}

void MainGame::drawGame() {
	// Reset screen
	glClearDepth(1.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	_colorProgram.use();
	glActiveTexture(GL_TEXTURE0);

	GLint textureLocation = _colorProgram.getUniformLocation("imageTexture");
	glUniform1i(textureLocation, 0);

	// GLuint timeLocation = _colorProgram.getUniformLocation("time");
	// glUniform1f(timeLocation, time);

	_sprite.draw();

	glBindTexture(GL_TEXTURE_2D, 0);
	_colorProgram.unuse();

	_window.swapBuffer();
}

void MainGame::calculateFPS() {
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
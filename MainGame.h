#pragma once

#include<SDL/SDL.h>
#include<GL/glew.h>

#include"GLSLProgram.h"
#include"Sprite.h"
#include "GLTexture.h"

enum class GameState {
	PLAY, EXIT
};

class MainGame {
public:
	MainGame();

	void run();

	float time;

private:
	void initSystems();
	void initShaders();
	void gameLoop();
	void processInput();
	void drawGame();
	void calculateFPS();

	SDL_Window* _window;
	GameState _gameState;
	int _screenWidth;
	int _screenHeight;

	GLSLProgram _colorProgram;

	float _fps;
	float _frameTime;
	float _maxFPS;

	// for testing: remove later
	Sprite _sprite;
};
#pragma once

#include<SDL/SDL.h>
#include<GL/glew.h>

#include"GLSLProgram.h"
#include"Sprite.h"

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

	SDL_Window* _window;
	GameState _gameState;
	int _screenWidth;
	int _screenHeight;

	GLSLProgram _colorProgram;

	// for testing: remove later
	Sprite _sprite;
};
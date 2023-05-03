#pragma once

#include<SDL/SDL.h>
#include<GL/glew.h>

enum class GameState {
	PLAY, EXIT
};

class MainGame
{
public:
	MainGame();

	void run();

private:
	void initSystems();
	void gameLoop();
	void processInput();
	void drawGame();

	SDL_Window* _window;
	GameState _gameState;
	int _screenWidth;
	int _screenHeight;
};


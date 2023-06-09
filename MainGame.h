#pragma once

#include<SDL/SDL.h>
#include<GL/glew.h>
#include<Jauntlet/GLSLProgram.h>
#include<Jauntlet/Sprite.h>
#include<Jauntlet/GLTexture.h>
#include<Jauntlet/Window.h>
#include<Jauntlet/Jauntlet.h>
#include<Jauntlet/Camera2D.h>

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

	Jauntlet::Window _window;
	GameState _gameState;
	int _screenWidth;
	int _screenHeight;

	Jauntlet::GLSLProgram _colorProgram;
	Jauntlet::Camera2D _camera;

	float _fps;
	float _frameTime;
	float _maxFPS;

	// for testing: remove later
	Jauntlet::Sprite _sprite;
};
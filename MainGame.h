#pragma once

#include<SDL/SDL.h>
#include<GL/glew.h>

#include<Jauntlet/GLSLProgram.h>
#include<Jauntlet/Sprite.h>
#include<Jauntlet/GLTexture.h>
#include<Jauntlet/Window.h>
#include<Jauntlet/Jauntlet.h>
#include<Jauntlet/Camera2D.h>
#include<Jauntlet/SpriteBatch.h>
#include<Jauntlet/InputManager.h>
#include<Jauntlet/Timing.h>

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

	Jauntlet::Window _window;
	GameState _gameState;
	int _screenWidth;
	int _screenHeight;

	Jauntlet::GLSLProgram _colorProgram;
	Jauntlet::Camera2D _camera;
	
	Jauntlet::SpriteBatch _spriteBatch;
	
	Jauntlet::InputManager _inputManager;
	Jauntlet::FpsLimiter _fpsLimiter;

	int _fps;
	int _maxFPS;

};
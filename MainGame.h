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
#include<Jauntlet/TileMap.h>

enum class GameState {
	PLAY, EXIT
};

class MainGame {
public:
	MainGame();

	void run();

private:
	void initSystems();
	void initShaders();
	void gameLoop();
	void processInput();
	void drawGame();

	Jauntlet::Window _window;

	Jauntlet::GLSLProgram _colorProgram;
	
	Jauntlet::Camera2D _camera;
	
	Jauntlet::InputManager _inputManager;

	GameState _gameState;

	int _screenWidth, _screenHeight;
	int _fps;

	// Temporary variables for testing

	Jauntlet::TileMap _level;
};
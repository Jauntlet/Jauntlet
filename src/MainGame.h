#pragma once

#include <GL/glew.h>
#include <Jauntlet/Camera2D.h>
#include <Jauntlet/InputManager.h>
#include <Jauntlet/Jauntlet.h>
#include <Jauntlet/Rendering/GLSLProgram.h>
#include <Jauntlet/Rendering/GLTexture.h>
#include <Jauntlet/Rendering/LineRenderer.h>
#include <Jauntlet/Rendering/Sprite.h>
#include <Jauntlet/Rendering/SpriteBatch.h>
#include <Jauntlet/Rendering/SpriteFont.h>
#include <Jauntlet/TileMap.h>
#include <Jauntlet/TileSet.h>
#include <Jauntlet/Time.h>
#include <Jauntlet/Window.h>
#include <SDL/SDL.h>

#include "Player.h"
#include "glm/fwd.hpp"

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
	void drawGame();
	void drawHUD();

	Jauntlet::Window _window;

	Jauntlet::GLSLProgram _colorProgram;
	
	Jauntlet::Camera2D _camera;
	
	Jauntlet::InputManager _inputManager;

	Jauntlet::TextureCache _textureCache;

	GameState _gameState = GameState::PLAY;

	// this is faster to compile, but much less readable.
	// arguably, we only *really* compile this once, and only recompile whenever we change constants. -jk

	int _screenWidth = 1024, _screenHeight = 768;
	float _fps = 0;

	glm::vec2 _currentCameraPostition = glm::vec2(0,0);
	glm::vec2 _intendedCameraPostition = glm::vec2(0,0);

	glm::vec2 _oldMouse = glm::vec2(0,0);
	glm::vec2 _deltaMouse = glm::vec2(0,0);

	// Temporary variables for testing
	Jauntlet::SpriteBatch _playerSpriteBatch;

	Jauntlet::TileMap _level;
	Jauntlet::TileSet _bricks;

	Jauntlet::SpriteBatch _HUDSpriteBatch;
	Jauntlet::SpriteFont* _spriteFont;
	
	Jauntlet::LineRenderer _lineRenderer;

	Player _player;
};
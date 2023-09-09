#include "MainGame.h"
#include "SDL/SDL.h"
#include "glm/fwd.hpp"

#include <Jauntlet/Jauntlet.h>
#include <Jauntlet/Errors.h>
#include <Jauntlet/Rendering/ResourceManager.h>
#include <Jauntlet/Time.h>
#include <Jauntlet/TileSet.h>
#include <Jauntlet/JMath.h>


#include <iostream>

// defines scale of movement for the camera. if set to 1, the camera will follow the mouse, if set to 0, the mouse has no control over the camera.
const float _CAMERA_MOVEMENT_SCALE =  0.5f; // DO NOT DEFINE IN HEADER, BREAKS CONST -jk

MainGame::MainGame() :
	_level(_textureCache, 32),
	_bricks("Textures/none.png", "Textures/all.png", "Textures/right.png", "Textures/left.png", "Textures/bottom.png", "Textures/top.png",
		"Textures/bottomRight.png", "Textures/bottomLeft.png", "Textures/bottomTop.png", "Textures/topRight.png", "Textures/topLeft.png", "Textures/rightLeft.png",
		"Textures/bottomTopLeft.png", "Textures/bottomTopRight.png", "Textures/bottomLeftRight.png", "Textures/topRightLeft.png"),
	_player(-5 * 32, 0, &_inputManager),
	_window() {
}

void MainGame::run() {
	initSystems();

	gameLoop();
}

void MainGame::initSystems() {
	Jauntlet::init();

	_window.create("Jauntlet Game Engine", _screenWidth, _screenHeight, Jauntlet::WindowFlags::RESIZEABLE);

	SDL_ShowCursor(1); // show the mouse cursor. can be set to 0 later for replacements.

	initShaders();

	_lineRenderer.init();

	_camera.init(_screenWidth, _screenHeight);
	_hudCamera.init(_screenWidth, _screenHeight);

	// initialize player spriteBatch
	_playerSpriteBatch.init();
	_HUDSpriteBatch.init();

	// initializes spritefont
	_spriteFont.init(&_hudCamera, "Fonts/HandelGo.ttf", 64);
	//_spriteFont.init("Fonts/chintzy.ttf", 64);

	// Temporary level loading
	_bricks.addConnectionRule(Jauntlet::TileSet::ConnectionRules::TILES);
	_level.registerTileSet('T', _bricks, Jauntlet::TileCollision::SQUARE);

	//_level.loadTileMap("Levels/level0.txt");
	_level.loadTileMap("Levels/testAllTiles.txt");
}

void MainGame::initShaders() {
	_colorProgram.compileShaders("Shaders/colorShading.vert", "Shaders/colorShading.frag");
	_colorProgram.addAttribute("vertexPosition");
	_colorProgram.addAttribute("vertexColor");
	_colorProgram.addAttribute("vertexUV");
	_colorProgram.linkShaders();
}

void MainGame::gameLoop() {
	Jauntlet::Time::setMaxFPS(60);

	while (_gameState == GameState::PLAY) {
		Jauntlet::Time::beginFrame();

		processInput();

		_player.update();
		
		// collision checking between Craig and the tilemap, done after player update
		std::vector<Jauntlet::BoxCollider2D> levelColliders = _level.collectCollidingTiles(_player.getPosition());
		Jauntlet::Collision2D collision = Jauntlet::Collision2D();
		for (int j = 0; j < levelColliders.size(); ++j) {
			if (collision.getCollision(&_player.collider, &levelColliders[j])) {
				_player.setPosition(_player.getPosition() + (collision.GetNormal() * collision.GetOverlap()));
			}
		}

		_camera.update();
		_hudCamera.update();

		drawGame();

		_fps = Jauntlet::Time::endFrame();
	}
}

void MainGame::processInput() {
	_inputManager.processInput();

	if (_inputManager.quitGameCalled()) {
		_gameState = GameState::EXIT;
	}

	if (_inputManager.isKeyDown(SDLK_LSHIFT) || _inputManager.isKeyDown(SDL_CONTROLLER_BUTTON_A)) {
		_player.setSpeed(300);
	}
	else {
		_player.setSpeed(120);
	}

	if (_inputManager.isKeyDown(SDLK_r)) {
		_camera.transitionToPosition(glm::vec2(0));
		_camera.transitionToScale(1);
	}

	if (_inputManager.isKeyDown(SDL_BUTTON_LEFT)) {
		_camera.clearTransitions();
		_deltaMouse = glm::vec2(_oldMouse.x - _inputManager.getMouseCoords().x, _inputManager.getMouseCoords().y - _oldMouse.y);
	} else {
		_deltaMouse -= _deltaMouse * (Jauntlet::Time::getDeltaTime() * 10);
	}

	_camera.translate(_deltaMouse);

	if (_inputManager.isKeyPressed(SDLK_F11) || (_inputManager.isKeyDown(SDLK_LALT) || _inputManager.isKeyDown(SDLK_RALT)) && _inputManager.isKeyPressed(SDLK_RETURN)) {
		_window.toggleFullscreen();
	}

	if (_inputManager.windowResized()) {
		_window.getWindowSize(); // This not only gets the window size, but also recalculates it incase of window resizing going unchecked.
		_screenWidth = _window.getWindowWidth();
		_screenHeight = _window.getWindowHeight();
		_camera.updateCameraSize(_screenWidth, _screenHeight);
		_hudCamera.updateCameraSize(_screenWidth, _screenHeight);
	}

	if (_inputManager.deltaScroll != 0) {
		_camera.clearTransitions();
		_camera.multiplyScale(pow(1.2f, _inputManager.deltaScroll));
		_inputManager.deltaScroll = 0;
	}

	//test for collider-position code
	if (_inputManager.isKeyPressed(SDL_BUTTON_RIGHT)) {
		Jauntlet::Collision2D data = Jauntlet::Collision2D();
		if (data.getCollision(&_player.collider, _camera.convertScreenToWorld(_inputManager.getMouseCoords()))) {
			std::cout << "Jollision @ " << _inputManager.getMouseCoords().x << ", " << _inputManager.getMouseCoords().y << std::endl;
		} 
	}

	_oldMouse = _inputManager.getMouseCoords(); // the old mouse position is now the current mouse position
}

void MainGame::drawGame() {
	// Reset screen
	glClearDepth(1.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	_colorProgram.use();
	
	glActiveTexture(GL_TEXTURE0);

	// Reading information into shaders
	glUniform1i(_colorProgram.getUniformLocation("imageTexture"), 0);
	glUniformMatrix4fv(_colorProgram.getUniformLocation("Projection"), 1, GL_FALSE, &_camera.getCameraMatrix()[0][0]);

	// Draw Level
	_level.draw();
	// Draw the player using a spriteBatch
	_playerSpriteBatch.begin();
	_player.draw(_playerSpriteBatch);
	_playerSpriteBatch.end();

	_playerSpriteBatch.renderBatch();

	_colorProgram.unuse();

	drawHUD();

	_window.swapBuffer();
}

void MainGame::drawHUD() {
	glUniformMatrix4fv(_colorProgram.getUniformLocation("Projection"), 1, GL_FALSE, &_hudCamera.getCameraMatrix()[0][0]);

	_HUDSpriteBatch.begin();

	std::string output = "Framerate: " + std::to_string((int)_fps);
	_spriteFont.draw(_HUDSpriteBatch, output, _hudCamera.convertScreenToWorld(glm::vec2(20, _spriteFont.getFontHeight())), glm::vec2(1), 0, Jauntlet::Color(255, 100, 100, 255));

	_HUDSpriteBatch.end();
	_HUDSpriteBatch.renderBatch();
}
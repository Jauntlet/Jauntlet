#include "MainGame.h"
#include "SDL/SDL_keycode.h"
#include "SDL/SDL_mouse.h"
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
	_player(-5 * 32, 0, &_inputManager) {
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

	// initialize player spriteBatch
	_playerSpriteBatch.init();
	_HUDSpriteBatch.init();

	// initializes spritefont
	//_spriteFont = new Jauntlet::SpriteFont("Fonts/chintzy.ttf", 64);

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
		std::vector<BoxCollider2D> levelColliders = _level.collectCollidingTiles(_player.getPosition());
		Jauntlet::Collision2D collision = Jauntlet::Collision2D();
		for (int j = 0; j < levelColliders.size(); ++j) {
			if (collision.getCollision(&_player.collider, &levelColliders[j])) {
				_player.setPosition(_player.getPosition() + (collision.GetNormal() * collision.GetOverlap()));
			}
		}

		_oldMouse = _inputManager.getMouseCoords(); // the old mouse position is now the current mouse position

		_currentCameraPostition += (_intendedCameraPostition - _currentCameraPostition) * (Jauntlet::Time::getDeltaTime() * 4);

		_camera.setPosition(_currentCameraPostition);

		_camera.update();

		drawGame();

		_fps = Jauntlet::Time::endFrame();
	}
}

void MainGame::processInput() {
	_inputManager.processInput();

	if (_inputManager.quitGameCalled()) {
		_gameState = GameState::EXIT;
	}

	if (_inputManager.isKeyDown(SDLK_LSHIFT)) {
		_player.setSpeed(300);
	}
	else {
		_player.setSpeed(120);
	}

	if (_inputManager.isKeyDown(SDLK_q)) {
		_camera.setScale(_camera.getScale() + .05);
	}
	if (_inputManager.isKeyDown(SDLK_e)) {
		_camera.setScale(_camera.getScale() - .05);
	}

	if (_inputManager.isKeyDown(SDLK_r)) {
		_intendedCameraPostition = glm::vec2(0, 0);
	}

	if (_inputManager.isKeyDown(SDL_BUTTON_LEFT)) {

		_deltaMouse = _inputManager.getMouseCoords() - _oldMouse;
		_currentCameraPostition += glm::vec2(-_deltaMouse.x, _deltaMouse.y);
		_intendedCameraPostition = _currentCameraPostition;
	}

	if (_inputManager.isKeyPressed(SDLK_F11) || (_inputManager.isKeyDown(SDLK_LALT) || _inputManager.isKeyDown(SDLK_RALT)) && _inputManager.isKeyPressed(SDLK_RETURN)) {
		_window.toggleFullscreen();
	}

	if (_inputManager.windowResized()) {
		_window.getWindowSize();
		_camera.updateCameraSize(_window.getWindowWidth(), _window.getWindowHeight());
	}
}

void MainGame::drawGame() {
	// Reset screen
	glClearDepth(1.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	_colorProgram.use();
	
	glActiveTexture(GL_TEXTURE0);

	// Reading information into shaders
	GLint textureUniform = _colorProgram.getUniformLocation("imageTexture");
	glUniform1i(textureUniform, 0);

	glm::mat4 projectionMatrix = _camera.getCameraMatrix();
	GLint pUniform = _colorProgram.getUniformLocation("P");
	glUniformMatrix4fv(pUniform, 1, GL_FALSE, &projectionMatrix[0][0]);

	// Draw Level
	_level.draw();
	// Draw the player using a spriteBatch
	_playerSpriteBatch.begin();
	_player.draw(_playerSpriteBatch);
	_playerSpriteBatch.end();

	_playerSpriteBatch.renderBatch();

	drawHUD();

	_lineRenderer.setColor(glm::vec3(1, 0, 0));
	_lineRenderer.drawLine(glm::vec2(-1, 0), glm::vec2(1, 0));

	_colorProgram.unuse();

	_window.swapBuffer();
}

void MainGame::drawHUD() {
	std::string output = "Framerate: " + std::to_string((int)_fps);

	_HUDSpriteBatch.begin();

	//_spriteFont->draw(_HUDSpriteBatch, output.c_str(), glm::vec2(32), glm::vec2(1), 0, Jauntlet::Color(255,255,255,255));

	_HUDSpriteBatch.end();
	_HUDSpriteBatch.renderBatch();
}
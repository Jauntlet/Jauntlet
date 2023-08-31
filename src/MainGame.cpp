#include "MainGame.h"

#include<Jauntlet/Jauntlet.h>
#include<Jauntlet/Errors.h>
#include<Jauntlet/Rendering/ResourceManager.h>
#include<Jauntlet/Time.h>
#include<Jauntlet/TileSet.h>

#include<iostream>

MainGame::MainGame() : 
	_screenWidth(1024), 
	_screenHeight(768), 
	_gameState(GameState::PLAY),
	_fps(0),
	_window(),
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

	initShaders();

	_lineRenderer.init();

	_camera.init(_screenWidth, _screenHeight);

	// initialize player spriteBatch
	_playerSpriteBatch.init();

	_spriteFont.init("Fonts/HandelGo.ttf", 32);

	// Temporary level loading
	_level.registerTile('B', "Textures/Craig.png");
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

		// process all input and detect if the player hits quit
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
		
		_player.update();

		//debug - send debug message when collision happens
		std::vector<BoxCollider2D> levelColliders = _level.collectCollidingTiles(_player.getPosition());
		Jauntlet::Collision2D collision = Jauntlet::Collision2D();
		for (int j = 0; j < levelColliders.size(); ++j) {
			if (collision.getCollision(&_player.collider, &levelColliders[j])) {
				//std::cout << "Collision Detected (player @ " << _player.getPosition().x << ", " << _player.getPosition().y << ")\n";
			}
		}

		// centers the camera on the player (16 is half the player width)
		_camera.setPosition(_player.getPosition() + glm::vec2(16,16));

		if (_inputManager.isKeyPressed(SDLK_F11) || (_inputManager.isKeyDown(SDLK_LALT) || _inputManager.isKeyDown(SDLK_RALT)) && _inputManager.isKeyPressed(SDLK_RETURN)) {		
			_window.toggleFullscreen();
		}

		if (_inputManager.windowResized()) {
			_window.getWindowSize();
			_camera.updateCameraSize(_window.getWindowWidth(), _window.getWindowHeight());
		}

		_camera.update();

		drawGame();

		_fps = Jauntlet::Time::endFrame();
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

	std::string str = "bruh";

	// Draw Level
	_level.draw();
	// Draw the player using a spriteBatch
	_playerSpriteBatch.begin();
	_player.draw(_playerSpriteBatch);
	_spriteFont.draw(_playerSpriteBatch, const_cast<char*>(str.c_str()), glm::vec2(32,32), glm::vec2(1,1), 0, Jauntlet::Color(255,255,255,255));
	_playerSpriteBatch.end();

	_playerSpriteBatch.renderBatch();

	_lineRenderer.setColor(glm::vec3(1, 0, 0));
	_lineRenderer.drawLine(glm::vec2(-1, 0), glm::vec2(1, 0));

	_colorProgram.unuse();

	_window.swapBuffer();
}
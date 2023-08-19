#include "MainGame.h"

#include<Jauntlet/Jauntlet.h>
#include<Jauntlet/Errors.h>
#include<Jauntlet/ResourceManager.h>
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
		"Textures/bottomTopLeft.png", "Textures/bottomTopRight.png", "Textures/bottomLeftRight.png", "Textures/topRightLeft.png") {
}

void MainGame::run() {
	initSystems();

	gameLoop();
}

void MainGame::initSystems() {
	Jauntlet::init();

	_window.create("Jauntlet Game Engine", _screenWidth, _screenHeight, 0);

	initShaders();

	_camera.init(_screenWidth, _screenHeight);

	// Temporary level loading
	_level.registerTile('B', "Textures/Craig.png");
	_bricks.addConnectionRule(Jauntlet::TileSet::ConnectionRules::TILES);
	_level.registerTileSet('T', _bricks);

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

	Jauntlet::Time Time;
	Time.setMaxFPS(60);

	while (_gameState == GameState::PLAY) {
		Time.beginFrame();

		// process all input and detect if the player hits quit
		if (_inputManager.processInput()) {
			_gameState = GameState::EXIT;
		}
		_camera.update();

		drawGame();

		_fps = Time.endFrame();
		std::cout << Time.getDeltaTime()<<":"<<_fps<< "\n";
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

	_colorProgram.unuse();

	_window.swapBuffer();
}
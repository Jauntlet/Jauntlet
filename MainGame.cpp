#include "MainGame.h"

#include<Jauntlet/Jauntlet.h>
#include<Jauntlet/Errors.h>
#include<Jauntlet/ResourceManager.h>
#include<Jauntlet/Timing.h>

#include<iostream>
MainGame::MainGame() : 
	_screenWidth(1024), 
	_screenHeight(768), 
	_gameState(GameState::PLAY),
	_fps(0),
	_window(),
	_level(32) {
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
	_level.registerTile('T', "Textures/test (2).png");
	_level.loadTileMap("Levels/level0.txt");

}

void MainGame::initShaders() {
	_colorProgram.compileShaders("Shaders/colorShading.vert", "Shaders/colorShading.frag");
	_colorProgram.addAttribute("vertexPosition");
	_colorProgram.addAttribute("vertexColor");
	_colorProgram.addAttribute("vertexUV");
	_colorProgram.linkShaders();
}

void MainGame::gameLoop() {

	Jauntlet::FpsLimiter fpsLimiter;
	fpsLimiter.setMaxFPS(60);

	while (_gameState == GameState::PLAY) {
		fpsLimiter.beginFrame();

		processInput();
		_camera.update();

		drawGame();

		_fps = fpsLimiter.endFrame();
	}
}

void MainGame::processInput() {
	SDL_Event evnt;
	
	while (SDL_PollEvent(&evnt))
	{
		switch (evnt.type)
		{
			case SDL_QUIT:
				_gameState = GameState::EXIT;
				break;
			case SDL_KEYDOWN:
				_inputManager.pressKey(evnt.key.keysym.sym);
				break;
			case SDL_KEYUP:
				_inputManager.releaseKey(evnt.key.keysym.sym);
				break;
			case SDL_MOUSEBUTTONDOWN:
				_inputManager.pressKey(evnt.button.button);
				break;
			case SDL_MOUSEBUTTONUP:
				_inputManager.releaseKey(evnt.button.button);
				break;
			case SDL_MOUSEMOTION:
				_inputManager.setMouseCoords(evnt.motion.x, evnt.motion.y);
				break;
		}
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
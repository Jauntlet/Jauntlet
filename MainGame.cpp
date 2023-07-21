#include "MainGame.h"
#include<iostream>
#include<string>

#include<Jauntlet/Errors.h>
#include<Jauntlet/ResourceManager.h>

MainGame::MainGame() : _screenWidth(1024), _screenHeight(768), _gameState(GameState::PLAY), time(0), _maxFPS(120), _fps(0), _window(), _camera() {

	_camera.init(_screenWidth, _screenHeight);
}

void MainGame::run() {
	initSystems();

	gameLoop();
}

void MainGame::initSystems() {

	Jauntlet::init();

	_window.create("Jauntlet", _screenWidth, _screenHeight, 0);

	initShaders();

	_spriteBatch.init();
	_fpsLimiter.init(_maxFPS);
}

void MainGame::initShaders() {
	_colorProgram.compileShaders("Shaders/colorShading.vert", "Shaders/colorShading.frag");
	_colorProgram.addAttribute("vertexPosition");
	_colorProgram.addAttribute("vertexColor");
	_colorProgram.addAttribute("vertexUV");
	_colorProgram.linkShaders();
}

void MainGame::gameLoop() {
	while (_gameState != GameState::EXIT) {
		
		_fpsLimiter.beginFrame();
		
		processInput();

		// temporary time
		time += 0.01;

		_camera.update();

		drawGame();

		_fps = _fpsLimiter.endFrame();

		// debug output FPS every 30 frames
		static int frameCounter = 0;
		frameCounter++;
		if (frameCounter == 30) {
			std::cout << std::round(_fps) << std::endl;
			frameCounter = 0;
		}
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
		}
	}

	if (_inputManager.isKeyDown(SDLK_w)) {
		_camera.setPosition(_camera.getPosition() + glm::vec2(0, 2));
	}
	if (_inputManager.isKeyDown(SDLK_s)) {
		_camera.setPosition(_camera.getPosition() + glm::vec2(0, -2));
	}
	if (_inputManager.isKeyDown(SDLK_a)) {
		_camera.setPosition(_camera.getPosition() + glm::vec2(-2, 0));
	}
	if (_inputManager.isKeyDown(SDLK_d)) {
		_camera.setPosition(_camera.getPosition() + glm::vec2(2, 0));
	}
	if (_inputManager.isKeyDown(SDLK_q)) {
		_camera.setScale(_camera.getScale() + 0.1);
	}
	if (_inputManager.isKeyDown(SDLK_e)) {
		_camera.setScale(_camera.getScale() - 0.1);
	}
}

void MainGame::drawGame() {
	// Reset screen
	glClearDepth(1.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	_colorProgram.use();
	glActiveTexture(GL_TEXTURE0);

	GLint textureLocation = _colorProgram.getUniformLocation("imageTexture");
	glUniform1i(textureLocation, 0);

	// GLuint timeLocation = _colorProgram.getUniformLocation("time");
	// glUniform1f(timeLocation, time);

	// Set the camera matrix
	GLuint pLocation = _colorProgram.getUniformLocation("P");
	glm::mat4 cameraMatrix = _camera.getCameraMatrix();

	glUniformMatrix4fv(pLocation, 1, GL_FALSE, &(cameraMatrix[0][0]));

	_spriteBatch.begin();

	glm::vec4 pos(0, 0, 50, 50);
	glm::vec4 uv(0, 0, 1, 1);
	Jauntlet::GLTexture texture = Jauntlet::ResourceManager::getTexture("Textures/Craig.png");
	Jauntlet::Color color;
	color.r = 255;
	color.g = 255;
	color.b = 255;
	color.a = 255;

	_spriteBatch.draw(pos, uv, texture.id, 0, color);
	_spriteBatch.draw(pos + glm::vec4(50, 0, 0, 0), uv, texture.id, 0, color);

	_spriteBatch.end();
	_spriteBatch.renderBatch();

	glBindTexture(GL_TEXTURE_2D, 0);
	_colorProgram.unuse();

	_window.swapBuffer();
}
#include "MainGame.h"
#include<iostream>
#include<string>
#include"Errors.h"

MainGame::MainGame() : _window(nullptr), _screenWidth(1024), _screenHeight(768), _gameState(GameState::PLAY), time(0) {
	
}

void MainGame::run() {
	initSystems();

	_sprite.init(-1, -1, 2, 2);

	gameLoop();
}

void MainGame::initSystems() {
	// Initialize SDL
	SDL_Init(SDL_INIT_EVERYTHING);

	// create the window
	_window = SDL_CreateWindow("Jauntlet Game Engine", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, _screenWidth, _screenHeight, SDL_WINDOW_OPENGL);
	if (_window == nullptr) {
		fatalError("SDL Window could not be created!");
	}
	// Have OpenGL work on the window
	SDL_GLContext glContext = SDL_GL_CreateContext(_window);
	if (glContext == nullptr) {
		fatalError("SDL_GL context could not be created!");
	}
	// enable GLEW for older PCs that don't support everything modern
	GLenum error = glewInit();
	if (error != GLEW_OK) {
		fatalError("Could not initialize glew!");
	}

	// allows for buffer swapping
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

	// sets the background color of window
	glClearColor(0.298f, 0.094f, 0.125f, 1);

	initShaders();
}

void MainGame::initShaders() {
	_colorProgram.compileShaders("Shaders/colorShading.vert", "Shaders/colorShading.frag");
	_colorProgram.addAttribute("vertexPosition");
	_colorProgram.addAttribute("vertexColor");
	_colorProgram.linkShaders();
}

void MainGame::gameLoop() {
	while (_gameState != GameState::EXIT) {
		processInput();

		// temporary time
		time += 0.01;

		drawGame();
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
		}
	}
}

void MainGame::drawGame() {
	// Reset screen
	glClearDepth(1.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	_colorProgram.use();

	GLuint timeLocation = _colorProgram.getUniformLocation("time");
	glUniform1f(timeLocation, time);

	_sprite.draw();

	_colorProgram.unuse();

	SDL_GL_SwapWindow(_window);
}
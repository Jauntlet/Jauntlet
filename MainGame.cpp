#include "MainGame.h"
#include<iostream>
#include<string>

#include"Errors.h"

MainGame::MainGame() : _window(nullptr), _screenWidth(1024), _screenHeight(768), _gameState(GameState::PLAY), time(0), _maxFPS(120) {

}

void MainGame::run() {
	initSystems();

	// debugging code: to be removed
	_sprite.init(-1, -1, 2, 2, "Textures/Craig.png");

	gameLoop();
}

void MainGame::initSystems() {
	// Initialize SDL
	SDL_Init(SDL_INIT_EVERYTHING);

	// allows for buffer swapping
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

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

	// tells you your version of OpenGL
	std::printf("*** OpenGL Version: %s ***\n", glGetString(GL_VERSION));

	// sets the background color of window
	glClearColor(0.298f, 0.094f, 0.125f, 1);

	// this turns on VSync (0 = off, 1 = on)
	SDL_GL_SetSwapInterval(0);

	initShaders();
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
		float startTicks = SDL_GetTicks();
		
		processInput();

		// temporary time
		time += 0.01;

		drawGame();
		calculateFPS();
		
		// debug output FPS every 10 frames
		static int frameCounter = 0;
		frameCounter++;
		if (frameCounter == 10) {
			std::cout << _fps << std::endl;
			frameCounter = 0;
		}

		// FPS limiting 
		float frameTicks = SDL_GetTicks() - startTicks;
		if (1000 / _maxFPS > frameTicks) {
			SDL_Delay(1000 / _maxFPS - frameTicks);
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
		}
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

	_sprite.draw();

	glBindTexture(GL_TEXTURE_2D, 0);
	_colorProgram.unuse();

	SDL_GL_SwapWindow(_window);
}

void MainGame::calculateFPS() {
	// NUM_SAMPLES is how many frames we are averaging by
	static const int NUM_SAMPLES = 10;
	static float frameTimes[NUM_SAMPLES] = {};
	static int currentFrame = 0;
	static float prevTicks = SDL_GetTicks();
	
	float currentTicks = SDL_GetTicks();

	_frameTime = currentTicks - prevTicks;
	frameTimes[currentFrame % NUM_SAMPLES] = _frameTime;

	prevTicks = currentTicks;

	int count = NUM_SAMPLES;

	currentFrame++;

	if (currentFrame < NUM_SAMPLES) {
		count = currentFrame;
	}

	float frameTimeAverage = 0;
	for (int i = 0; i < count; i++) {
		frameTimeAverage += frameTimes[i];
	}
	frameTimeAverage /= count;

	if (frameTimeAverage > 0) {
		_fps = 1000 / frameTimeAverage;
	}
	else {
		_fps = 0;
	}
}
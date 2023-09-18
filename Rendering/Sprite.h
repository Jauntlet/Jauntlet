#pragma once
// THIS CLASS IS OLD AND DOESNT WORK WITH SPRITEBATCHING, SO IT SHOULD NOT BE USED: READ DOCS FOR MORE INFO
// THIS CLASS IS OLD AND DOESNT WORK WITH SPRITEBATCHING, SO IT SHOULD NOT BE USED: READ DOCS FOR MORE INFO
// THIS CLASS IS OLD AND DOESNT WORK WITH SPRITEBATCHING, SO IT SHOULD NOT BE USED: READ DOCS FOR MORE INFO
#include<GL/glew.h>
#include<string>

#include"GLTexture.h"

namespace Jauntlet {
	class Sprite
	{
	public:
		Sprite(float x, float y, float width, float height, std::string texturePath);
		// this is a destructor
		~Sprite();

		// draws the texture to the screen
		void draw();

	private:
		float _x;
		float _y;
		float _width;
		float _height;
		GLuint _vboID = 0;

		GLTexture _texture;
	};
}
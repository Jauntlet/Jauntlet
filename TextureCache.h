#pragma once
#include<map>
#include<string>

#include "GLTexture.h"
class TextureCache
{
public:
	TextureCache();

	GLTexture getTexture(std::string filePath);
private:
	std::map<std::string, GLTexture> _textureMap;
};


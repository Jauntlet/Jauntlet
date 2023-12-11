#pragma once
#include "Textures/SpriteBatch.h"
#include "Textures/ResourceManager.h"

namespace Jauntlet {
class ProgressBar {
public:
	ProgressBar(const std::string& texture1, const glm::vec4& texture1UV, const std::string& texture2, const glm::vec4& texture2UV, const glm::vec4& dest);
	ProgressBar(const std::string& texture, const glm::vec4& texture1UV, const glm::vec4& texture2UV, const glm::vec4& dest);

	void setProgress(const float& newProgress);
	void setPosition(glm::vec2 newPosition);

	void draw(SpriteBatch& batch);
private:
	GLuint _emptyTexture, _fullTexture;
	glm::vec4 _UV1, _UV2;
	glm::vec2 _position, _size;

	float _progress = 0;
};
}


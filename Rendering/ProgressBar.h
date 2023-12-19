#pragma once
#include "Textures/SpriteBatch.h"
#include "Textures/ResourceManager.h"

namespace Jauntlet {
class ProgressBar {
public:
	ProgressBar(const std::string& texture1, const glm::vec4& texture1UV, const std::string& texture2, const glm::vec4& texture2UV, const glm::vec4& dest);
	ProgressBar(const std::string& texture, const glm::vec4& texture1UV, const glm::vec4& texture2UV, const glm::vec4& dest);

	virtual void draw(SpriteBatch& batch);

	glm::vec2 position;
	float progress = 0;
protected:
	GLuint _emptyTexture, _fullTexture;
	glm::vec4 _UV1, _UV2;
	glm::vec2 _size;
};
}


#pragma once
#include<glm/glm.hpp>

namespace Jauntlet {
class LineRenderer {
public:
	LineRenderer();
	// destructor
	~LineRenderer();
private:
	glm::mat4 _MVP;
	int _shaderProgram;

	unsigned int _VAO, _VBO;
};
};

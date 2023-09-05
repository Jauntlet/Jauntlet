#pragma once

#include <glm/glm.hpp>
#include <vector>

namespace Jauntlet {
class LineRenderer {
public:
	LineRenderer();
	// initialize line renderer
	void init();
	// destructor
	~LineRenderer();
	void setColor(glm::vec3);
	void drawLine(glm::vec2 start, glm::vec2 end);
private:
	glm::mat4 _MVP;
	int _shaderProgram;

	unsigned int _VAO, _VBO;

	std::vector<float> _vertices;

	glm::vec3 _color;
};
};

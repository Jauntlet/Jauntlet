#pragma once

#include <string>
#include <glm/glm.hpp>

namespace Jauntlet {
class AnimationFrame
{
public:
	enum TileSides { RIGHT = 0x1, BOTTOM = 0x2, TOP = 0x4, LEFT = 0x8 };
	// The rules for AnimationFrames connecting to other tile types. NONE means it will only connect to itself.
	enum ConnectionRules { NONE = 0x0, AnimationFrameS = 0x1, TILES = 0x2, EMPTY = 0x4 };

	struct FrameInfo {
		std::string texture;
		glm::vec4 UV;

		FrameInfo() {
			texture = "";
			UV = glm::vec4(0, 0, 1, 1);
		}
	};

	AnimationFrame(std::string filePath, glm::vec2 spriteSize);
	// This constructor assumes a single image with a specific layout, please see examples provided.
	AnimationFrame(std::string FilePath);

	void play(int start, int end, float frameTime);
	void update(float deltaTime);
	void stop();
	glm::vec4 getUV();
private:

	float _elapsedTime = 0;
	
	glm::vec2 _spriteSize;

	int _start;
	int _end;

	int _frame;

	float _frameTime;

	bool _playing = false;

	glm::vec4 _uv;
};
}
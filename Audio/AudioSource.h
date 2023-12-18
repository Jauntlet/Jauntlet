/* Purpose: Loads and plays sounds at a specified location
 * Main Contributer(s): Xander Mooney
 */
#pragma once
#include <string>
#include <AL/al.h>
#include <glm/vec3.hpp>
#include <vector>
namespace Jauntlet {
class AudioSource
{
public:
	AudioSource(const glm::vec3& position = glm::vec3(0));

	void update();

	bool playWAV(const std::string& sound);
private:
	std::vector<std::pair<ALuint, ALuint>> _sources;
	glm::vec3 _position = glm::vec3(0);
};
}


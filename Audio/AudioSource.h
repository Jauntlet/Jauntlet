/* Purpose: Loads and plays sounds at a specified location
 * Main Contributer(s): Xander Mooney
 */
#pragma once
#include <string>
#include <AL/al.h>
#include <glm/vec3.hpp>
#include <vector>

const size_t NUM_BUFFERS = 4;
const size_t BUFFER_SIZE = 65536; // 32kb of data in each buffer

namespace Jauntlet {
struct AudioStream {
	std::string fileName;
	ALuint source;
	ALuint buffers[NUM_BUFFERS];
	uint8_t channels;
	int32_t sampleRate;
	uint8_t bitsPerSample;
	ALsizei size;
	ALenum format;
	size_t cursor = 0;
	std::vector<char> soundData;
	bool processing = true; // reads false if no more data needs to be pushed to it.
};

class AudioSource
{
public:
	AudioSource(const glm::vec3& position = glm::vec3(0));

	void update();

	bool playWAV(const std::string& sound);
private:
	std::vector<AudioStream> _sources;
	glm::vec3 _position = glm::vec3(0);
};

}
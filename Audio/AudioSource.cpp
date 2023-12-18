#include "AudioSource.h"
#include "../Filesystems/FileManager.h"
#include "../Errors.h"
using namespace Jauntlet;

AudioSource::AudioSource(const glm::vec3& position) {
	_position = position;
}

void AudioSource::update() {
	ALint state = AL_PLAYING;
	for (size_t i = 0; i < _sources.size(); ++i) {
		alGetSourcei(_sources[i].first, AL_SOURCE_STATE, &state);
		if (state == AL_STOPPED) {
			alDeleteSources(1, &_sources[i].first);
			alDeleteBuffers(1, &_sources[i].second);
		}
	}
}

bool AudioSource::playWAV(const std::string& sound) {
	uint8_t channels;
	int32_t sampleRate, size;
	uint8_t bitsPerSample;
	char* soundData;

	soundData = FileManager::readWAVFile(sound, channels, sampleRate, bitsPerSample, size);

	if (soundData == nullptr) {
		Jauntlet::error("Could not load WAV \"" + sound + "\"");
		return false;
	}

	ALuint buffer;
	alGenBuffers(1, &buffer);
	ALenum format;
	if (channels == 1 && bitsPerSample == 8) {
		format = AL_FORMAT_MONO8;
	} else if (channels == 1 && bitsPerSample == 16) {
		format = AL_FORMAT_MONO16;
	} else if (channels == 2 && bitsPerSample == 8) {
		format = AL_FORMAT_STEREO8;
	} else if (channels == 2 && bitsPerSample == 16) {
		format = AL_FORMAT_STEREO16;
	} else {
		Jauntlet::error("Unrecognized format of " + std::to_string(channels) + " channels, " + std::to_string(bitsPerSample) + " bps");
		return false;
	}

	alBufferData(buffer, format, soundData, size, sampleRate);

	// set source values
	ALuint source;
	alGenSources(1, &source);
	alSourcef(source, AL_PITCH, 1.0f);
	alSourcef(source, AL_GAIN, 1.0f);
	alSource3f(source, AL_POSITION, _position.x, _position.y, _position.z);
	alSource3f(source, AL_VELOCITY, 0, 0, 0);
	alSourcei(source, AL_LOOPING, AL_FALSE);
	alSourcei(source, AL_BUFFER, buffer);

	// play source
	alSourcePlay(source);
	// store source and buffer for later processing
	_sources.emplace_back(source, buffer);
}
#include "AudioSource.h"
#include "../Filesystems/FileManager.h"
#include "../Errors.h"

#ifdef __linux
#include <cstring>
#endif

AudioSource::AudioSource(const glm::vec3& position) {
	_position = position;
}
AudioSource::~AudioSource() {
	for (size_t i = 0; i < _sources.size(); ++i) {
		alDeleteSources(1, &_sources[i].source);
		alDeleteBuffers(NUM_BUFFERS, &_sources[i].buffers[0]);
	}
}

void AudioSource::update() {
	ALint buffersProcessed = 0;

	for (size_t i = 0; i < _sources.size(); ++i) {
		alGetSourcei(_sources[i].source, AL_BUFFERS_PROCESSED, &buffersProcessed);
		if (buffersProcessed > 0) {
			while (buffersProcessed--) {
				ALuint buffer;
				alSourceUnqueueBuffers(_sources[i].source, 1, &buffer);
				
				char* data = new char[BUFFER_SIZE];
				std::memset(data, 0, BUFFER_SIZE);

				size_t dataSize = BUFFER_SIZE;
				if (_sources[i].cursor + BUFFER_SIZE > _sources[i].size) {
					dataSize = _sources[i].size - _sources[i].cursor;
				}

				if (dataSize != 0) {
					std::memcpy(&data[0], &_sources[i].soundData[_sources[i].cursor], dataSize);
					_sources[i].cursor += dataSize;
				}
				
				if (dataSize < BUFFER_SIZE) {
					if (_sources[i].looping) {
						_sources[i].cursor = 0;
					} else {
						int buffersLeft = 0;
						alGetSourcei(_sources[i].source, AL_BUFFERS_QUEUED, &buffersLeft);

						if (buffersLeft == 0) {
							alDeleteSources(1, &_sources[i].source);
							alDeleteBuffers(NUM_BUFFERS, &_sources[i].buffers[0]);
							_sources.erase(_sources.begin() + i);
							--i;
							break;
						}
						continue;
					}
				}
				
				alBufferData(buffer, _sources[i].format, data, dataSize, _sources[i].sampleRate);
				alSourceQueueBuffers(_sources[i].source, 1, &buffer);
				
				delete[] data;
			}
		}
	}
}

void AudioSource::setPosition(const glm::vec3& newPos) {
	_position = newPos;

	for (size_t i = 0; i < _sources.size(); ++i) {
		alSource3f(_sources[i].source, AL_POSITION, _position.x, _position.y, _position.z);
	}
}
glm::vec3 AudioSource::getPosition() {
	return _position;
}

bool AudioSource::playWAV(const std::string& sound, bool loops) {
	AudioStream stream;
	stream.looping = loops;

	stream.soundData = FileManager::readWAVFile(sound, stream);

	if (stream.soundData.empty()) {
		Jauntlet::error("Could not load WAV \"" + sound + "\"");
		return false;
	}

	// gen buffers into stream
	alGenBuffers(NUM_BUFFERS, &stream.buffers[0]);

	// set correct format
	if (stream.channels == 1 && stream.bitsPerSample == 8) {
		stream.format = AL_FORMAT_MONO8;
	} else if (stream.channels == 1 && stream.bitsPerSample == 16) {
		stream.format = AL_FORMAT_MONO16;
	} else if (stream.channels == 2 && stream.bitsPerSample == 8) {
		stream.format = AL_FORMAT_STEREO8;
	} else if (stream.channels == 2 && stream.bitsPerSample == 16) {
		stream.format = AL_FORMAT_STEREO16;
	} else {
		Jauntlet::error("Unrecognized format of " + std::to_string(stream.channels) + " channels, " + std::to_string(stream.bitsPerSample) + " bps");
		Jauntlet::error("Could not play WAV: \"" + sound + "\"");
		return false;
	}

	// if BUFFER_SIZE * NUM_BUFFERS is smaller than the audios file size, we want to divide the audio evenly between the 4 buffers.
	int bufSize = BUFFER_SIZE * NUM_BUFFERS < stream.size ? BUFFER_SIZE : stream.size / NUM_BUFFERS;
	
	for (size_t i = 0; i < NUM_BUFFERS; ++i) {
		alBufferData(stream.buffers[i], stream.format, &stream.soundData[stream.cursor], bufSize, stream.sampleRate);
		stream.cursor += bufSize;
	}

	// set source values
	alGenSources(1, &stream.source);
	alSourcef(stream.source, AL_PITCH, 1.0f);
	alSourcef(stream.source, AL_GAIN, 1.0f);
	alSource3f(stream.source, AL_POSITION, _position.x, _position.y, _position.z);
	alSource3f(stream.source, AL_VELOCITY, 0, 0, 0);

	// attach buffers and play source
	alSourceQueueBuffers(stream.source, NUM_BUFFERS, &stream.buffers[0]);
	alSourcePlay(stream.source);
	// store source for later processing
	_sources.push_back(stream);
	return true;
}
#include "AudioSource.h"
#include "../Filesystems/FileManager.h"
#include "../Errors.h"

using namespace Jauntlet;

AudioSource::AudioSource(const glm::vec3& position) {
	_position = position;
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
				
				int looping = 0;
				alGetSourcei(_sources[i].source, AL_LOOPING, &looping);

				size_t dataSize = BUFFER_SIZE;
				if (_sources[i].cursor + BUFFER_SIZE > _sources[i].soundData.size()) {
					dataSize = _sources[i].soundData.size() - _sources[i].cursor;
				}

				if (dataSize == 0) {
					_sources[i].processing = false;
					
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

				std::memcpy(&data[0], &_sources[i].soundData[_sources[i].cursor], dataSize);
				_sources[i].cursor += dataSize;

				
				if (dataSize < BUFFER_SIZE) {
					if (looping == AL_TRUE) {
						_sources[i].cursor = 0;
						std::memcpy(&data[dataSize], &_sources[i].soundData[_sources[i].cursor], BUFFER_SIZE - dataSize);
					}
					else {
						_sources[i].cursor = _sources[i].soundData.size();
					}
				}
		
				alBufferData(buffer, _sources[i].format, data, BUFFER_SIZE, _sources[i].sampleRate);
				alSourceQueueBuffers(_sources[i].source, 1, &buffer);
				
				delete[] data;
			}
		}
	}
}

bool AudioSource::playWAV(const std::string& sound) {
	AudioStream stream;

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

	// This assumes that the soundData is bigger than NUM_BUFFERS * BUFFER_SIZE. FIX THIS 
	for (size_t i = 0; i < NUM_BUFFERS; ++i) {
		//if (stream.cursor + BUFFER_SIZE <= stream.cursor) {}
		alBufferData(stream.buffers[i], stream.format, &stream.soundData[i * BUFFER_SIZE], BUFFER_SIZE, stream.sampleRate);
		stream.cursor += BUFFER_SIZE;
	}

	// set source values
	alGenSources(1, &stream.source);
	alSourcef(stream.source, AL_PITCH, 1.0f);
	alSourcef(stream.source, AL_GAIN, 1.0f);
	alSource3f(stream.source, AL_POSITION, _position.x, _position.y, _position.z);
	alSource3f(stream.source, AL_VELOCITY, 0, 0, 0);
	alSourcei(stream.source, AL_LOOPING, AL_FALSE);
	//alSourcei(source, AL_BUFFER, buffer);

	// attach buffers and play source
	alSourceQueueBuffers(stream.source, NUM_BUFFERS, &stream.buffers[0]);
	alSourcePlay(stream.source);
	// store source for later processing
	_sources.push_back(stream);
	return true;
}
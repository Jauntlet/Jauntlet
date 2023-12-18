#include "AudioListener.h"
#include <cstddef>

#include <iostream> // REMOVE

using namespace Jauntlet;

AudioListener::AudioListener() {
	 _device = alcOpenDevice(NULL);
     _context = alcCreateContext(_device, NULL);
	 alcMakeContextCurrent(_context);

     checkALErrors();

};
AudioListener::~AudioListener() {
    _context = alcGetCurrentContext();
    _device = alcGetContextsDevice(_context);
    alcMakeContextCurrent(NULL);
    alcDestroyContext(_context);
    alcCloseDevice(_device);
}

std::string AudioListener::getDefaultDeviceName() {
	return alcGetString(NULL, ALC_DEFAULT_DEVICE_SPECIFIER);
}
std::string AudioListener::getDeviceNames() {
	return alcGetString(NULL, ALC_DEVICE_SPECIFIER);
}

glm::vec3 AudioListener::getPosition() {
    glm::vec3 pos = glm::vec3();
    alGetListener3f(AL_POSITION, &pos.x, &pos.y, &pos.z);
    return pos;
}
void AudioListener::setPosition(const glm::vec3& pos) {
    alListener3f(AL_POSITION, pos.x, pos.y, pos.z);
}

float AudioListener::getGain() {
    float gain = 0.0f;
    alGetListenerf(AL_GAIN, &gain);
    return gain;
}
void AudioListener::setGain(float gain) {
    alListenerf(AL_GAIN, gain);
}

glm::vec3 AudioListener::getVelocity() {
    glm::vec3 velocity = glm::vec3();
    alGetListener3f(AL_VELOCITY, &velocity.x, &velocity.y, &velocity.z);
    return velocity;
}
void AudioListener::setVelocity(const glm::vec3& velocity) {
    alListener3f(AL_VELOCITY, velocity.x, velocity.y, velocity.z);
}

bool AudioListener::checkALErrors() {
	ALenum ALerror = alGetError();
	if (ALerror != AL_NO_ERROR) {
		switch (ALerror) {
        case AL_INVALID_NAME:
            fatalError("AL_INVALID_NAME: a bad name (ID) was passed to an OpenAL function");
            break;
        case AL_INVALID_ENUM:
            fatalError("AL_INVALID_ENUM: an invalid enum value was passed to an OpenAL function");
            break;
        case AL_INVALID_VALUE:
            fatalError("AL_INVALID_VALUE: an invalid value was passed to an OpenAL function");
            break;
        case AL_INVALID_OPERATION:
            fatalError("AL_INVALID_OPERATION: the requested operation is not valid");
            break;
        case AL_OUT_OF_MEMORY:
            fatalError("AL_OUT_OF_MEMORY: the requested operation resulted in OpenAL running out of memory");
            break;
        default:
            error("UNKNOWN AL ERROR: " + std::to_string(ALerror));
        }
        return false;
	}
    return true;
}
bool AudioListener::checkALCErrors() {
    ALCenum ALCerror = alcGetError(_device);
    if (ALCerror != ALC_NO_ERROR) {
        switch (ALCerror)
        {
        case ALC_INVALID_VALUE:
            fatalError("ALC_INVALID_VALUE: an invalid value was passed to an OpenAL function");
            break;
        case ALC_INVALID_DEVICE:
            fatalError("ALC_INVALID_DEVICE: a bad device was passed to an OpenAL function");
            break;
        case ALC_INVALID_CONTEXT:
            fatalError("ALC_INVALID_CONTEXT: a bad context was passed to an OpenAL function");
            break;
        case ALC_INVALID_ENUM:
            fatalError("ALC_INVALID_ENUM: an unknown enum value was passed to an OpenAL function");
            break;
        case ALC_OUT_OF_MEMORY:
            fatalError("ALC_OUT_OF_MEMORY: an unknown enum value was passed to an OpenAL function");
            break;
        default:
            fatalError("UNKNOWN ALC ERROR: " + std::to_string(ALCerror));
        }
        return false;
    }
    return true;
}
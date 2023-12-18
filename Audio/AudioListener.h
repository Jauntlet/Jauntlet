/* Purpose: Manages Sounds and represents the point in which you hear sounds from
 * Main Contributer(s): Xander Mooney / Jack Kennedy
 */
#pragma once
#include <AL/al.h>
#include <AL/alc.h>
#include "../Errors.h"
#include <glm/vec3.hpp>
#include "../Filesystems/FileManager.h"

namespace Jauntlet {
class AudioListener
{
public:
	AudioListener();
	~AudioListener();

	std::string getDefaultDeviceName();
	std::string getDeviceNames();

	glm::vec3 getPosition();
	void setPosition(const glm::vec3& pos);

	float getGain();
	void setGain(float gain);

	glm::vec3 getVelocity();
	void setVelocity(const glm::vec3& pos);

	// Missing functions for accessing the "orientation" of the Listener. This is a Vector6 value, likely split into two Vector3s.
	// In the documentation its described as "at" and "up" vectors, but this does not make it any easier to implement.
	// This feature will remain untouched until Jauntlet goes 3D, where we can actually test this -xm
	//??? getOrientation();
	//void setOrientation(const ??? pos);
private:
	bool checkALErrors();
	bool checkALCErrors();
	ALCdevice* _device;
	ALCcontext* _context;
};
}
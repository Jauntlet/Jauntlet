/* Purpose: Sets up, stores, and plays back animations through textures and their UVS.
 * Main Contributer(s): Jack Kennedy, Xander Mooney
 */

#pragma once

#include <glm/fwd.hpp>
#include <glm/vec4.hpp>
#include <string>

namespace Jauntlet {
class Animation
{
public:
	Animation(unsigned int frames);
	// Play an animation.
	// start: starting index of animation
	// end: ending index of animation
	// frameTime: time that must pass for next frame to play
	// loop: whether the animation should loop or not. True by default.
	void play(int start, int end, float frameTime, bool loop = true);
	// Update the animation; should be played every frame
	// returns true if an animation is currently active
	bool update();
	// stop the animation.
	void stop();
	// stop the animation and set it to a specific frame.
	void stop(int frame);
	// pauses the animation.
	void pause();
	// pauses the animation and changes it to the desired frame.
	void pause(int frame);
	// resumes the animation and continues playing.
	void resume();
	// returns the correct UVs for the animation to be applied at runtime.
	glm::vec4 getUV() const;
	// returns the current frame the animation is on.
	int getCurrentFrame() const;
private:
	float _elapsedTime = 0;
	
	float _spriteSize;

	int _start;
	int _end;

	int _frame = -1;

	float _frameTime;

	bool _playing = false;
	bool _loop = false;

	glm::vec4 _uv = glm::vec4(0,0,1,1);

	// recalculate the uv
	void recalculateUV(); // DONTDOCUMENT
};
}
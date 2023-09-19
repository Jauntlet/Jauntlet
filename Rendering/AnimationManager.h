#pragma once

#include <glm/fwd.hpp>
#include <glm/glm.hpp>
#include <string>
#include <vector>

#include "Animation.h"

namespace Jauntlet {
class AnimationManager
{
public:
	AnimationManager();
    void update();
    void add(Animation* animation);
private:

	std::vector<Animation*> _animations;
};
}
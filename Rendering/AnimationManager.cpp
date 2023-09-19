#include "AnimationManager.h"

using namespace Jauntlet;

AnimationManager::AnimationManager() {
	// blank
}

void AnimationManager::add(Animation* animation) {
    _animations.push_back(animation);
}

void AnimationManager::update() {
    for (int i = 0; i < _animations.size(); ++i) {
        _animations[i]->update();
    }
}
#include "ParticleGrow.h"

ParticleGrow::ParticleGrow(float startSize, float endSize) {
	_startSize = startSize;
    _sizeDiff = startSize - endSize;
}

void ParticleGrow::apply(std::vector<Particle>& emissions) {
    for (size_t i = 0; i < emissions.size(); ++i) {
        emissions[i].size = glm::vec2(_startSize - _sizeDiff * (emissions[i].age / emissions[i].lifetime));
    }
}
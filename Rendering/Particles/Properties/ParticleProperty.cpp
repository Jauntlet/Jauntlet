#include "ParticleProperty.h"
#include "../../../Errors.h"

ParticleProperty::ParticleProperty() {
	// empty
}

void ParticleProperty::apply(std::vector<Particle>& emissions) {
    Jauntlet::error("ParticleProperty.apply() was called! this is a generic particle property, not to be called! its apply statement will forever do nothing!");
}
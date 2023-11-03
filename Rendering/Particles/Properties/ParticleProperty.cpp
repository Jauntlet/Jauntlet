#include "ParticleProperty.h"
#include "../../../Errors.h"

using namespace Jauntlet;

ParticleProperty::ParticleProperty(float values[]) {
	// empty
}

void ParticleProperty::apply() {
    error("ParticleProperty.apply() was called! this is a generic particle property, not to be called! its apply statement will forever do nothing!");
}
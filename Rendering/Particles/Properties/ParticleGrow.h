#pragma once

#include "ParticleProperty.h"

namespace Jauntlet {
class ParticleGrow : public ParticleProperty
{
public:
    // constructor example of a particle property.
	ParticleGrow(float startSize, float endSize); // UNDOCUMENTED

    // method to only be used by a particle to apply the property onto said particle
    void apply(std::vector<Emission> emissions) override; // DONTDOCUMENT
private:
    float _startSize = 0;
    float _sizeDiff = 10;
};
}
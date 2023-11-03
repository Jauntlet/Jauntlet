#pragma once

namespace Jauntlet {
class ParticleProperty
{
public:
    // constructor example of a particle property.
	ParticleProperty(float values[]); // UNDOCUMENTED

    // method to only be used by a particle to apply the property onto said particle
    void apply(); // DONTDOCUMENT
private:
    
};
}
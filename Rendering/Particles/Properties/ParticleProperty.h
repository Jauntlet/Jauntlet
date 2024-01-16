/* Purpose: Base method to handle all possible properties for particles
 * Main Contributer(s): Jack Kennedy
 */
#pragma once

#include "../Emission.h"

class ParticleProperty
{
public:
    // constructor example of a particle property.
	ParticleProperty(); // UNDOCUMENTED

    // method to only be used by a particle to apply the property onto said particle
    virtual void apply(std::vector<Emission>& emissions); // DONTDOCUMENT
private:
    
};
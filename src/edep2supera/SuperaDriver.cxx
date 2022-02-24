

#include "SuperaDriver.h"
#include "supera/data/Particle.h"
#include <iostream>

namespace edep2supera { 


	void SuperaDriver::ReadEvent(const TG4Event* ev)
	{
		std::cout<<ev<<std::endl;
		supera::ParticleInput pg();
	}

}
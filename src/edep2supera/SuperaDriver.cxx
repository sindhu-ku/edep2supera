

#include "SuperaDriver.h"
#include <iostream>

namespace supera { 


	void SuperaDriver::ReadEvent(const TG4Event* ev)
	{
		std::cout<<ev<<std::endl;
		supera::ParticleGroup pg();
	}

}
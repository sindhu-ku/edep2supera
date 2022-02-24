#ifndef __SUPERADRIVER_H__
#define __SUPERADRIVER_H__

#include "EDepSim/TG4Event.h"
#include "supera/data/Particle.h"

namespace edep2supera {

	class SuperaDriver
	{
	public:
		SuperaDriver() {}

		void ReadEvent(const TG4Event* ev);
		supera::Particle TG4TrajectoryToParticle(const TG4Trajectory& edepsim_part);
		supera::ProcessType InferProcessType(const TG4Trajectory& edepsim_part);
	};
}
#endif

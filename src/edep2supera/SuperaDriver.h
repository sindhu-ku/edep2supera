

#include "EDepSim/TG4Event.h"
#include "supera/base/SuperaData.h"

namespace supera {

	class SuperaDriver {
	public:
		SuperaDriver() : val(0) {}

		void ReadEvent(const TG4Event* ev);

	};
}

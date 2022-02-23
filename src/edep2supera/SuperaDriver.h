

#include "EDepSim/TG4Event.h"
#include "supera/base/SuperaData.h"
#include "supera/base/SuperaEvent.h"

namespace supera
{

	class SuperaDriver
	{
	public:
		SuperaDriver() : val(0) {}

		EventInput ReadEvent(const TG4Event *ev);
	};
}

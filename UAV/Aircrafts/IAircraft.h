#pragma once

#include <Structures/PulseWidthSet.h>
#include <Structures/AngularControlTrust.h>

namespace IoT {
	namespace UAV {

		class IAircraft {
		public:
			virtual void configure(const AngularControlTrust& angularControlTrust, PulseWidthSet& dataValue) = 0;
			
			virtual void setAirflow(const AngularControlTrust & angularControlTrust, PulseWidthSet& dataValue) = 0;

			virtual void setIdle(PulseWidthSet& dataValue) = 0;

			float clamp(float f, float min = 0.0f, float max = 1.0f) const {
				return std::min(std::max(f, min), max);
			}
		};
	}
}

#pragma once

#include <Thread/AtomicMethod.hpp>
#include <Logger.h>
#include "IAircraft.h"

namespace IoT {
	namespace UAV {

		class Quadrocopter
			: public IAircraft
		{
			enum Wings {
				FrontRight = 0,
				RearRight = 1,
				RearLeft = 2,
				FrontLeft = 3
			};

		public:
			explicit Quadrocopter(float minimumSpeed)
				: m_minimumSpeed(minimumSpeed)
			{}

			void configure(const AngularControlTrust& angularControlTrust, PulseWidthSet& dataValue) override;
			
			void setAirflow(const AngularControlTrust& angularControlTrust, PulseWidthSet& dataValue) override;

			void setStableAirflow(float thrust, float yaw, float pitch, float roll, PulseWidthSet& dataValue) const;

			void setIdle(PulseWidthSet& dataValue) override;

		private:
			Core::ILogger *m_logger{Core::ILogger::getDefault()};
			float m_minimumSpeed;
		};
	}
}

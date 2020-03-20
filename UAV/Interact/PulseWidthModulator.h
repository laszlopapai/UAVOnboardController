#pragma once

#include <ILogger.h>
#include <Thread/AsyncProcess.h>
#include <Thread/AtomicMethod.hpp>
#include <Data/StreamNode.h>
#include <PCA9685/PCA9685PwmController.h>
#include "../Structures/PulseWidthSet.h"

namespace IoT {
	namespace UAV {

		class PulseWidthModulator
			: public Core::StreamNode<PulseWidthSet>
		{
		public:
			explicit PulseWidthModulator(const float outputFrequency);

			bool initialize();

			void run(Core::DataInfo<PulseWidthSet>& dataInfo);

		private:
			void update(Core::DataInfo<PulseWidthSet>& dataInfo);

		private:
			Core::ILogger *m_logger;

			uint64_t m_updateDelay;

			Device::PCA9685PwmController m_pulseWidthModulator;
		};

	}
}

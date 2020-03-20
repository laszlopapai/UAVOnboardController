#pragma once

#include <memory>

#include <Thread/AsyncProcess.h>
#include <L3GD20H/L3GD20HGyroscope.h>
#include <Thread/NotifyPipe.hpp>
#include <ILogger.h>

#include <Data/StreamNode.h>
#include <Filter/KalmanFilterVector3.hpp>

namespace IoT {
	namespace UAV {

		class Gyroscope
			: public Core::StreamNode<Core::Vector3>
		{
		public:
			explicit Gyroscope(const float outputFrequency);

			bool initialize();

			void run(Core::DataInfo<Core::Vector3>& dataInfo);

		private:
			Core::ILogger *m_logger;

			float m_outputFrequency;
			uint64_t m_acquireDelay;
			std::unique_ptr<Device::KalmanFilterVector3> m_filter;
			Device::L3GD20HGyroscope m_gyroscope;
		};

	}
}

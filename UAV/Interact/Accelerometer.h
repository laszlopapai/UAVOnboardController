#pragma once

#include <memory>

#include <Thread/AsyncProcess.h>
#include <LSM303D/LSM303DAccelerometer.h>
#include <Filter/AccelerometerFilter.h>
#include <Thread/NotifyPipe.hpp>
#include <ILogger.h>

#include<Data/StreamNode.h>
#include <Filter/KalmanFilterVector3.hpp>

namespace IoT {
	namespace UAV {

		class Accelerometer
			: public Core::StreamNode<Core::Vector3>
		{
		public:
			explicit Accelerometer(const float outputFrequency);

			bool initialize();

			void run(Core::DataInfo<Core::Vector3>& dataInfo);

		private:
			Core::ILogger *m_logger;

			float m_outputFrequency;
			uint64_t m_acquireDelay;
			std::unique_ptr<Device::KalmanFilterVector3> m_filter;
			Device::LSM303DAccelerometer m_accelerometer;
		};

	}
}

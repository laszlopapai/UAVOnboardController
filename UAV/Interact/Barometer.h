#pragma once

#include <ILogger.h>
#include <Thread/AsyncProcess.h>
#include <BMP180/BarometerData.h>
#include <BMP180/BMP180Barometer.h>
#include <Thread/AtomicMethod.hpp>

namespace IoT {
	namespace UAV {

		class Barometer
			: public Core::AsyncProcess
		{
		public:
			Barometer();

			bool initialize() override;

			void run() override;

			Core::NotifyPipe<Device::BarometerData> & getOutput()
			{
				return m_output;
			}

		protected:
			Core::Time deltaTime() const
			{
				return Core::Time::getNow() - m_lastAcquire;
			}

		private:
			Core::ILogger *m_logger;

			Core::Time m_lastAcquire;
			uint64_t m_acquireDelay;

			Device::BMP180Barometer m_barometer;
			Core::NotifyPipe<Device::BarometerData> m_output;

			Device::BarometerData m_currentData;
		};

	}
}

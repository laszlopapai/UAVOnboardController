#pragma once

#include <cstdint>
#include <ILogger.h>
#include <Thread/NotifyPipe.hpp>
#include <Thread/AsyncProcess.h>
#include <Thread/AwaitStopSignal.h>

namespace IoT {
	namespace Device {

		class HCSR04Rangefinder
			: public Core::AsyncProcess
		{
			const float c = 343.0f; // m / s The velocity of the sound
			const float maxDistanceWait = 2 / (c * .5f); // Maximum 2m distance

		public:
			HCSR04Rangefinder(uint32_t trigPin, uint32_t echoPin);

			bool initialize() override;

			Core::NotifyPipe<float> & getRangeOut() {
				return m_rangeOut;
			}

			void onStop() override;

		private:
			void run() override;

		private:
			uint32_t m_trigPin;
			uint32_t m_echoPin;

			Core::ILogger *m_logger;
			Core::AwaitStopSignal m_acquireSignal;
			Core::NotifyPipe<float> m_rangeOut;
		};
	}
}

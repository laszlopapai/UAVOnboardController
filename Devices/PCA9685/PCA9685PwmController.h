#pragma once

#include "PCA9685.h"

namespace IoT {
	namespace Device {

		class PCA9685PwmController
		{
			const uint32_t scale = 4095;

		private:
			PCA9685 m_pwm;
			uint32_t m_frequency;

		public:
			PCA9685PwmController(uint32_t frequency);

			PCA9685PwmController(uint32_t frequency, uint32_t address);

			bool initialize();

			void setValue(uint32_t channel, float high);

		};
	}
}

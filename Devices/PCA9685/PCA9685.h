#pragma once

#include <cstdint>
#include "wiringPiI2C.h"
#include <math.h>
#include <wiringPi.h>

namespace IoT {
	namespace Device {

		class PCA9685
		{

		public:
			PCA9685(uint32_t address = 0x40)
				: m_address(address)
				, m_device(0)
			{}

			bool inicialize();

			void setFrequency(uint32_t frequency);

			void setValue(uint32_t channel, uint32_t high, uint32_t low);

			void setValue(uint32_t high, uint32_t low);

		private:
			uint32_t m_address;
			int m_device;
		};
	}
}

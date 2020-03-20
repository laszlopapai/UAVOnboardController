#pragma once

#include "wiringPi.h"
#include "wiringPiI2C.h"
#include "BarometerData.h"
#include <math.h>
#include <stdio.h>

#include <cstdint>
#include <functional>

namespace IoT {
	namespace Device {
			
		class BMP180Barometer
		{
			enum OperatingModes : uint8_t {
				BMP085_ULTRALOWPOWER = 0,
				BMP085_STANDARD = 1,
				BMP085_HIGHRES = 2,
				BMP085_ULTRAHIGHRES = 3
			};
			enum Registers : uint8_t {
				BMP085_CAL_AC1 = 0xAA,  // R   Calibration data (16 bits)
				BMP085_CAL_AC2 = 0xAC,  // R   Calibration data (16 bits)
				BMP085_CAL_AC3 = 0xAE,  // R   Calibration data (16 bits)
				BMP085_CAL_AC4 = 0xB0,  // R   Calibration data (16 bits)
				BMP085_CAL_AC5 = 0xB2,  // R   Calibration data (16 bits)
				BMP085_CAL_AC6 = 0xB4,  // R   Calibration data (16 bits)
				BMP085_CAL_B1 = 0xB6,  // R   Calibration data (16 bits)
				BMP085_CAL_B2 = 0xB8,  // R   Calibration data (16 bits)
				BMP085_CAL_MB = 0xBA,  // R   Calibration data (16 bits)
				BMP085_CAL_MC = 0xBC,  // R   Calibration data (16 bits)
				BMP085_CAL_MD = 0xBE,  // R   Calibration data (16 bits)
				BMP085_CONTROL = 0xF4,
				BMP085_TEMPDATA = 0xF6,
				BMP085_PRESSUREDATA = 0xF6,
				auto_increment = 0x80
			};
			enum Commands : uint8_t {
				BMP085_READTEMPCMD = 0x2E,
				BMP085_READPRESSURECMD = 0x34
			};
				
		public:
			explicit BMP180Barometer(uint32_t address = 0x77)
				: m_address(address)
			{}

			bool initialize();

			void acquire(BarometerData &data);
				
		private:
			uint16_t read16(int device, int reg) {
				return ((uint8_t) wiringPiI2CReadReg8(device, reg) << 8) |
					(uint8_t) wiringPiI2CReadReg8(device, reg + 1);
			}

			long readRawPressure();

			long readRawTemperature();

		private:
			uint32_t m_address;
			uint8_t m_mode{ OperatingModes::BMP085_STANDARD };
			int m_device{ 0 };

			int16_t AC1{ 0 }, AC2{ 0 }, AC3{ 0 };
			uint16_t AC4{ 0 }, AC5{ 0 }, AC6{ 0 };
			int16_t VB1{ 0 }, VB2{ 0 };
			int16_t MB{ 0 }, MC{ 0 }, MD{ 0 };
		};
	}
}

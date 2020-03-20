#pragma once

#include <cstdint>
#include <Math/Vector3.h>

namespace IoT {
	namespace Device {

		class L3GD20HGyroscope
		{
		public:
			enum OutputDataRate : uint8_t {
				/// Represents an output data rate of 95Hz.
						Rate95Hz = 0x0,
				/// Represents an output data rate of 190Hz.
						Rate190Hz = 0x1,
				/// Represents an output data rate of 380Hz.
						Rate380Hz = 0x2,
				/// Represents an output data rate of 760Hz.
						Rate760Hz = 0x3
			};

			enum FullScale : uint8_t {
				/// Represents a full scale of 250 degrees-per-second (dps).
						Scale250dps = 0x0,
				/// Represents a full scale of 500 degrees-per-second (dps).
						Scale500dps = 0x1,
				/// Represents a full scale of 1000 degrees-per-second (dps).
						Scale1000dps = 0x2,
				/// Represents a full scale of 2000 degrees-per-second (dps).
						Scale2000dps = 0x3,
			};

		private:
			enum Registers : uint8_t {
				CTRL_REG1 = 0x20,
				CTRL_REG4 = 0x23,
				OUT_X_L = 0x28,
				auto_increment = 0x80
			};

			const uint16_t c_scales[4]{
					250,
					500,
					1000,
					2000
			};

			const uint16_t c_frequencies[4]{
					95,
					190,
					380,
					760
			};

		public:
			explicit L3GD20HGyroscope(uint32_t address = 0x69);

			bool initialize();

			void acquire(Core::Vector3 &data);

			uint16_t getFrequency() const {
				return m_frequency;
			}

		private:
			void writeCtrlReg1(OutputDataRate outputDataRate, bool enableZ = true, bool enableY = true,
				bool enableX = true);

			void writeCtrlReg4(FullScale scale);


		private:
			int32_t m_device{0};
			uint32_t m_address{0};
			uint16_t m_scale{0};
			uint16_t m_frequency{0};
		};
	}
}

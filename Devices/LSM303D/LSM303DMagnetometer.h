#pragma once

#include <cstdint>
#include <Math/Vector3.h>

namespace IoT {
	namespace Device {

		class LSM303DMagnetometer
		{
		public:
			enum OutputDataRate : uint8_t {
				/// Represents an output data rate of 0.755Hz.
						Rate0Hz = 0x0,

				/// Represents an output data rate of 1.5Hz.
						Rate1Hz = 0x1,

				/// Represents an output data rate of 3.0Hz.
						Rate3Hz = 0x2,

				/// Represents an output data rate of 7.5Hz.
						Rate7Hz = 0x3,

				/// Represents an output data rate of 15Hz.
						Rate15Hz = 0x4,

				/// Represents an output data rate of 30Hz.
						Rate30Hz = 0x5,

				/// Represents an output data rate of 75Hz.
						Rate75Hz = 0x6,

				/// Represents do not use.
						DoNotUse = 0x7
			};

			enum FullScale : uint8_t {
				/// Represents a full scale of 1.3G.
						Scale1d3G = 0x1,

				/// Represents a full scale of 1.9G.
						Scale1d9G = 0x2,

				/// Represents a full scale of 2.5G.
						Scale2d5G = 0x3,

				/// Represents a full scale of 4G.
						Scale4G = 0x4,

				/// Represents a full scale of 4.7G.
						Scale4d7G = 0x5,

				/// Represents a full scale of 5.6G.
						Scale5d6G = 0x6,

				/// Represents a full scale of 8.1G.
						Scale8d1G = 0x7
			};

			enum SensorMode : uint8_t {
				/// Represents continuous-conversion mode.
						ContinuousConversion = 0x0,

				/// Represents single-conversion mode.
						SingleConversion = 0x1,

				/// Represents power-down mode.
						PowerDown1 = 0x2,

				/// Represents sleep mode.
						PowerDown2 = 0x3
			};

		private:
			enum Registers : uint8_t {
				CTRL_REG_M0 = 0x00,
				CTRL_REG_M1 = 0x01,
				CTRL_REG_M2 = 0x02,

				OUT_X_L_M = 0x03,
				auto_increment = 0x80
			};

			const float c_scales[7]{
					1.3f,
					1.9f,
					2.5f,
					4.0f,
					4.7f,
					5.6f,
					8.1f
			};

			const uint16_t c_frequencies[8]{
					0,
					1,
					3,
					7,
					15,
					30,
					75,
					0
			};

		public:
			explicit LSM303DMagnetometer(uint32_t address = 0x1e);

			bool initialize();

			void acquire(Core::Vector3 &data);

			uint16_t getFrequency() {
				return m_frequency;
			}

		private:
			void writeCtrlReg5(OutputDataRate outputDataRate);

			void writeCtrlReg6(FullScale fullScale);

			void writeCtrlReg7(SensorMode mode);

		private:
			uint32_t m_address{0};
			int m_device{0};
			float m_scale{0};
			uint16_t m_frequency{0};
			uint32_t m_lastAcq{0};
		};
	}
}

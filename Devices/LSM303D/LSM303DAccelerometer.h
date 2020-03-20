#pragma once

#include <cstdint>
#include <Math/Vector3.h>

namespace IoT {
	namespace Device {

		class LSM303DAccelerometer
		{

		public:
			enum OutputDataRate : uint8_t {
				/// Represents power-down mode.
						PowerDown = 0x0,

				/// Represents an output data rate of 3.125Hz.
						Rate3Hz = 0x1,

				/// Represents an output data rate of 6.25Hz.
						Rate6Hz = 0x2,

				/// Represents an output data rate of 12.5Hz.
						Rate12Hz = 0x3,

				/// Represents an output data rate of 25Hz.
						Rate25Hz = 0x4,

				/// Represents an output data rate of 50Hz.
						Rate50Hz = 0x5,

				/// Represents an output data rate of 100Hz.
						Rate100Hz = 0x6,

				/// Represents an output data rate of 200Hz.
						Rate200Hz = 0x7,

				/// Represents an output data rate of 400Hz.
						Rate400Hz = 0x8,

				/// Represents an output data rate of 800Hz.
						Rate800Hz = 0x9,

				/// Represents an output data rate of 1600Hz.
						Rate1600Hz = 0xa
			};

			enum AntiAliasFilterBandwidth : uint8_t {
				/// Represents an anti-alias filter bandwidth of 773Hz.
						Bandwidth773Hz = 0x0,

				/// Represents an anti-alias filter bandwidth of 194Hz.
						Bandwidth194Hz = 0x1,

				/// Represents an anti-alias filter bandwidth of 362Hz.
						Bandwidth362Hz = 0x2,

				/// Represents an anti-alias filter bandwidth of 50Hz.
						Bandwidth50Hz = 0x3
			};

			enum FullScale : uint8_t {
				/// Represents a full scale of 2g.
						Scale2g = 0x0,

				/// Represents a full scale of 4g.
						Scale4g = 0x1,

				/// Represents a full scale of 8g.
						Scale6g = 0x2,

				/// Represents a full scale of 8g.
						Scale8g = 0x3,

				/// Represents a full scale of 16g.
						Scale16g = 0x4
			};

		private:
			enum Registers : uint8_t {
				CTRL_REG1 = 0x20,
				CTRL_REG2 = 0x21,
				OUT_X_L_A = 0x28,
				auto_increment = 0x80
			};

			const uint16_t c_scales[5]{
					2,
					4,
					6,
					8,
					16
			};

			const uint16_t c_frquencies[11]{
					0,
					3,
					6,
					12,
					25,
					50,
					100,
					200,
					400,
					800,
					1600
			};

		public:
			explicit LSM303DAccelerometer(uint32_t address = 0x19);

			bool initialize();

			void acquire(Core::Vector3 &data);

			uint16_t getFrequency() {
				return m_frequency;
			}

		private:
			void writeCtrlReg1(OutputDataRate outputDataRate, bool enableZ = true, bool enableY = true,
				bool enableX = true);

			void writeCtrlReg2(AntiAliasFilterBandwidth aliasFilterBandwidth, FullScale fullScale);

		private:
			int m_device{0};
			uint32_t m_address{0};
			uint16_t m_scale{0};
			uint16_t m_frequency{0};
			uint32_t m_lastAcq{0};
		};
	}
}

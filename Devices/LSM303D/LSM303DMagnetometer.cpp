#include <wiringPiI2C.h>
#include <climits>
#include "LSM303DMagnetometer.h"

using namespace IoT::Device;

LSM303DMagnetometer::LSM303DMagnetometer(uint32_t address)
	: m_address(address)
{ }

bool LSM303DMagnetometer::initialize() {
	if ((m_device = wiringPiI2CSetup(m_address)) < 0)
		return false;

	writeCtrlReg5(OutputDataRate::Rate75Hz);
	writeCtrlReg6(FullScale::Scale8d1G);
	writeCtrlReg7(SensorMode::ContinuousConversion);

	return true;
}

void LSM303DMagnetometer::acquire(Core::Vector3 & data) {
	uint8_t address = OUT_X_L_M | auto_increment;
	int16_t value;

	value = (int16_t)(
			wiringPiI2CReadReg8(m_device, address + 0) << 8 |
			wiringPiI2CReadReg8(m_device, address + 1)
	);
	data.x = value / (float)SHRT_MAX * m_scale;


	value = (int16_t)(
			wiringPiI2CReadReg8(m_device, address + 2) << 8 |
			wiringPiI2CReadReg8(m_device, address + 3)
	);
	data.y = value / (float)SHRT_MAX * m_scale;


	value = (int16_t)(
			wiringPiI2CReadReg8(m_device, address + 4) << 8 |
			wiringPiI2CReadReg8(m_device, address + 5)
	);
	data.z = value / (float)SHRT_MAX * m_scale;
}

void LSM303DMagnetometer::writeCtrlReg5(OutputDataRate outputDataRate) {
	wiringPiI2CWriteReg8(m_device, CTRL_REG_M0, outputDataRate << 2);
	m_frequency = c_frequencies[outputDataRate];
}

void LSM303DMagnetometer::writeCtrlReg6(FullScale fullScale) {
	wiringPiI2CWriteReg8(m_device, CTRL_REG_M1, fullScale << 5);
	m_scale = c_scales[fullScale - 1];
}

void LSM303DMagnetometer::writeCtrlReg7(SensorMode mode) {
	wiringPiI2CWriteReg8(m_device, CTRL_REG_M2, mode);
}

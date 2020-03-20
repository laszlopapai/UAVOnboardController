#include <climits>
#include <wiringPiI2C.h>
#include "LSM303DAccelerometer.h"
#include "../Filter/TimedVector3.h"

using namespace IoT::Device;

LSM303DAccelerometer::LSM303DAccelerometer(uint32_t address)
	: m_address(address)
{ }

bool LSM303DAccelerometer::initialize()
{
	if ((m_device = wiringPiI2CSetup (m_address)) < 0)
		return false;

	writeCtrlReg1(OutputDataRate::Rate400Hz);
	writeCtrlReg2(AntiAliasFilterBandwidth::Bandwidth194Hz, FullScale::Scale4g);

	return true;
}

void LSM303DAccelerometer::acquire(Core::Vector3 & data) {
	uint8_t address = OUT_X_L_A | auto_increment;

	data.x = (int16_t)wiringPiI2CReadReg16(m_device, address + 0) / (float)SHRT_MAX * m_scale;
	data.y = (int16_t)wiringPiI2CReadReg16(m_device, address + 2) / (float)SHRT_MAX * m_scale;
	data.z = (int16_t)wiringPiI2CReadReg16(m_device, address + 4) / (float)SHRT_MAX * m_scale;
}

void LSM303DAccelerometer::writeCtrlReg1(OutputDataRate outputDataRate, bool enableZ, bool enableY, bool enableX) {
	auto data = (uint8_t)(
		outputDataRate << 4 |
		(enableZ ? 0x1 << 2 : 0x0) |
		(enableY ? 0x1 << 1 : 0x0) |
		(enableX ? 0x1 << 0 : 0x0)
		);

	m_frequency = c_frquencies[outputDataRate];
	wiringPiI2CWriteReg8(m_device, CTRL_REG1, data);
}

void LSM303DAccelerometer::writeCtrlReg2(AntiAliasFilterBandwidth aliasFilterBandwidth, FullScale fullScale) {
	auto data = (uint8_t)(
		aliasFilterBandwidth << 6 |
		fullScale << 3
		);

	wiringPiI2CWriteReg8(m_device, CTRL_REG2, data);
	m_scale = c_scales[fullScale];
}

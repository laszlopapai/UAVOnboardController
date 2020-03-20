#include "L3GD20HGyroscope.h"
#include <wiringPi.h>
#include <wiringPiI2C.h>
#include <climits>


using namespace IoT::Device;

L3GD20HGyroscope::L3GD20HGyroscope(uint32_t address)
	: m_address(address)
{ }


bool L3GD20HGyroscope::initialize() {
	if ((m_device = wiringPiI2CSetup (m_address)) < 0)
		return false;

	writeCtrlReg1(OutputDataRate::Rate380Hz);
	writeCtrlReg4(FullScale::Scale2000dps);
	return true;
}

void L3GD20HGyroscope::acquire(Core::Vector3 &data) {
	uint8_t address = OUT_X_L | auto_increment;

	data.x = (int16_t)wiringPiI2CReadReg16(m_device, address + 0) / (float)SHRT_MAX * m_scale;
	data.y = (int16_t)wiringPiI2CReadReg16(m_device, address + 2) / (float)SHRT_MAX * m_scale;
	data.z = (int16_t)wiringPiI2CReadReg16(m_device, address + 4) / (float)SHRT_MAX * m_scale;
}

void L3GD20HGyroscope::writeCtrlReg1(OutputDataRate outputDataRate, bool enableZ, bool enableY, bool enableX) {
	auto data = (uint8_t)(
		outputDataRate << 6 |
		0x2 << 4 | //Bandwidth (BW)
		0x1 << 3 | //Power-down (PD) = Normal
		(enableZ ? 0x1 << 2 : 0x0) |
		(enableY ? 0x1 << 1 : 0x0) |
		(enableX ? 0x1 << 0 : 0x0)
		);

	wiringPiI2CWriteReg8(m_device, CTRL_REG1, data);
	m_frequency = c_frequencies[outputDataRate];
}

void L3GD20HGyroscope::writeCtrlReg4(FullScale scale) {
	m_scale = c_scales[scale];
	wiringPiI2CWriteReg8(m_device, CTRL_REG4, scale << 4);
}

#include <stdexcept>
#include "PCA9685.h"

using namespace IoT::Device;

bool PCA9685::inicialize() {
	if ((m_device = wiringPiI2CSetup (m_address)) < 0)
		return false;

	return true;
}

void PCA9685::setFrequency(uint32_t frequency) {

	wiringPiI2CWriteReg8(m_device, 0x0/*MODE1*/, 0x10/*SLEEP = 1 (Low Power Mode)*/ );

	int prescale = (int)round(25000000.0f / (4096.0f * frequency)) - 1;

	wiringPiI2CWriteReg8(m_device, 0xfe/*PRE_SCALE*/, prescale);


	wiringPiI2CWriteReg8(m_device, 0x0/*MODE1*/, 0x0/*SLEEP = 0 (Normal Mode)*/ );

	delayMicroseconds(5);

	wiringPiI2CWriteReg8(m_device, 0x0/*MODE1*/, 0xa0/*RESTART = 1 (Enabled), AI (auto-increment) = 1 (Enabled)*/ );


	wiringPiI2CReadReg8(m_device, 0xfe/*PRE_SCALE*/ );
}


void PCA9685::setValue(uint32_t channel, uint32_t high, uint32_t low) {
	if (channel < 0 || channel > 15) {
		return;

		throw std::invalid_argument("channel is out of range");
	}

	if (high < 0 || high > 4095) {
		return;

		throw std::invalid_argument("high value is out of range");
	}

	if (low < 0 || low > 4095) {
		return;

		throw std::invalid_argument("low value is out of range");
	}

	if (low < high) {
		return;

		throw std::invalid_argument("the low value cannot be higher than high value");
	}


	int regAddress = 0x6/*LED0_ON_L*/ + 4 * channel;
	wiringPiI2CWriteReg16(m_device, regAddress + 0, 0xfff & high);
	wiringPiI2CWriteReg16(m_device, regAddress + 2, 0xfff & low );
}
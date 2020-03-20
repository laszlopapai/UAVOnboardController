#include <algorithm>
#include <iostream>
#include "PCA9685PwmController.h"

using namespace IoT::Device;

PCA9685PwmController::PCA9685PwmController(uint32_t frequency)
	: m_pwm()
	, m_frequency(frequency)
{ }

PCA9685PwmController::PCA9685PwmController(uint32_t  frequency, uint32_t address)
	: m_pwm(address)
	, m_frequency(frequency)
{ }

bool PCA9685PwmController::initialize() {
	if (!m_pwm.inicialize()) {
		std::cerr << "Error during initialize PCA9685." << std::endl;
		return false;
	}
	m_pwm.setFrequency(m_frequency);
	return true;
}

void PCA9685PwmController::setValue(uint32_t channel, float high) {

	float wavelength = 1000.0f / m_frequency;
	uint32_t low = std::min(std::max((uint32_t)(high / wavelength * scale), UINT32_C(0)), scale);


	m_pwm.setValue(channel, 0, low);
}
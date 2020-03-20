#include "Config.h"
#include "DeviceHandler.h"
#include "Aircrafts/Quadrocopter.h"
#include <wiringPi.h>

using namespace IoT::UAV;

DeviceHandler::DeviceHandler(const float frequency)
	: m_logger(Core::ILogger::getDefault())
	, m_gyroscope(frequency)
	, m_accelerometer(frequency)
	, m_magnetometer(frequency)
	, m_pulseWidthModulator(frequency)
	, m_rangefinder(7, 1)
	, m_rotorSpeedTransform([this](auto& dataInfo) { this->setRotorSpeed(dataInfo); })
	, m_aircraft(std::make_unique<Quadrocopter>(.13f))
{ }

bool DeviceHandler::initialize() {

	// TODO: Error messages

	wiringPiSetup();

	if (!m_gyroscope.initialize()) {
		return false;
	}

	if (!m_accelerometer.initialize()) {
		return false;
	}

	if (!m_magnetometer.initialize()) {
		return false;
	}

	if (!m_pulseWidthModulator.initialize()) {
		return false;
	}

	if (!m_barometer.initialize()) {
		return false;
	}

	if (RotorAllowed) {
		m_rotorSpeedTransform | m_pulseWidthModulator;
	}

	return true;
}

void DeviceHandler::setRotorSpeed(Core::DataInfo<PulseWidthSet>& dataInfo) const
{
	if (getSignal() & ConfigMode) {
		AngularControlTrust angularControlTrust =
		{
			.angles = m_remoteSignal.current().getSetPoint(),
			.thrust = m_remoteSignal.current().getThrust()
		};
		m_aircraft->configure(angularControlTrust, dataInfo.m_data);
	}
	else {
		m_aircraft->setAirflow(dataInfo.getSource<AngularControlTrust>().current(), dataInfo.m_data);
	}

	dataInfo.m_propagate = true;
}

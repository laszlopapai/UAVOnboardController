#include "Magnetometer.h"
#include <Filter/MagnetometerFilter.h>

using namespace IoT::UAV;

Magnetometer::Magnetometer(const float outputFrequency)
	: StreamNode<Core::Vector3>([this](auto& dataInfo) { this->run(dataInfo); })
	, m_logger(Core::ILogger::getDefault())
	, m_outputFrequency(outputFrequency)
	, m_acquireDelay(0)
{ }

bool Magnetometer::initialize()
{
	if (!m_magnetometer.initialize()) {
		m_logger->log(Core::Error, "InputDevice", "Magnetometer initialize failed");
		return false;
	}
	m_logger->log(Core::Info, "InputDevice", "Magnetometer frequency: " + std::to_string(m_magnetometer.getFrequency()));

	m_filter = std::make_unique<Device::KalmanFilterVector3>(
		0.05f,
		0.0f,
		0.5f * m_outputFrequency / m_magnetometer.getFrequency(),
		0.0f,
		1.0f
	);
	m_acquireDelay = 1e6f / m_magnetometer.getFrequency();

	return true;
}

void Magnetometer::run(Core::DataInfo<Core::Vector3>& dataInfo)
{
	const auto currentDelay = ellapsed();
	if (currentDelay.getMicros() >= m_acquireDelay) {

		m_magnetometer.acquire(dataInfo.m_data);
		assert(!dataInfo.m_data.isNaN());

		m_filter->filter(dataInfo.m_data);

		if (currentDelay.getMicros() > m_acquireDelay * 2) {
			m_logger->log(Core::Warning, "InputDevice", "Magnetometer delay was " +
				std::to_string(currentDelay.getSecs<float>()) + "s. This is " +
				std::to_string(currentDelay.getMicros() / m_acquireDelay) +
				" times greater than the optimal!");
		}
		dataInfo.m_propagate = true;
	}
	else
		dataInfo.m_propagate = false;
}

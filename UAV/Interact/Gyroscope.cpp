#include "Gyroscope.h"
#include <Filter/GyroscopeFilter.h>

using namespace IoT::Core;
using namespace IoT::UAV;

Gyroscope::Gyroscope(const float outputFrequency)
	: StreamNode<Vector3>([this](auto& dataInfo) { this->run(dataInfo); })
	, m_logger(ILogger::getDefault())
	, m_outputFrequency(outputFrequency)
	, m_acquireDelay(0)
{ }

bool Gyroscope::initialize()
{
	if (!m_gyroscope.initialize()) {
		m_logger->log(Core::Error, "InputDevice", "Gyroscope initialize failed");
		return false;
	}
	m_logger->log(Core::Info, "InputDevice", "Gyroscope frequency: " + std::to_string(m_gyroscope.getFrequency()));

	m_filter = std::make_unique<Device::KalmanFilterVector3>(
		0.0000025f,
		0.0f,
		0.5f * m_outputFrequency / m_gyroscope.getFrequency(),
		0.0f,
		1.0f
	);

	m_acquireDelay = 1e6f / m_gyroscope.getFrequency();

	return true;
}

void Gyroscope::run(DataInfo<Vector3>& dataInfo)
{
	const auto currentDelay = ellapsed();
	if (currentDelay.getMicros() >= m_acquireDelay) {

		m_gyroscope.acquire(dataInfo.m_data);

		m_filter->filter(dataInfo.m_data);
		
		if (currentDelay.getMicros() > m_acquireDelay * 2) {
			m_logger->log(Core::Warning, "InputDevice", "Gyroscope delay was " +
				std::to_string(currentDelay.getSecs<float>()) + "s. This is " +
				std::to_string(currentDelay.getMicros() / m_acquireDelay) +
				" times greater than the optimal!");
		}
		dataInfo.m_propagate = true;
	}
	else
		dataInfo.m_propagate = false;
}

#include "Barometer.h"

using namespace IoT::UAV;

Barometer::Barometer()
	: m_logger(Core::ILogger::getDefault())
	, m_lastAcquire(0)
	, m_acquireDelay(0)
{ }

bool Barometer::initialize()
{
	if (!m_barometer.initialize()) {
		m_logger->log(Core::Error, "InputDevice", "Magnetometer initialize failed");
		return false;
	}

	m_acquireDelay = 1e6f / 1;

	return true;
}

void Barometer::run()
{
	const auto acquireTime = Core::Time::getNow();
	const auto currentDelay = acquireTime - m_lastAcquire;
	if (currentDelay.getMicros() >= m_acquireDelay) {

		if (m_lastAcquire.getMicros() > 0) { // The first delta would contains the init time
			m_barometer.acquire(m_currentData);
			m_output.pushData(m_currentData, deltaTime());

			if (currentDelay.getMicros() > m_acquireDelay * 2) {
				m_logger->log(Core::Warning, "InputDevice", "Barometer delay was " +
					std::to_string(currentDelay.getSecs()) + "s. This is " +
					std::to_string(currentDelay.getMicros() / m_acquireDelay) +
					" times greater than the optimal!");
			}
		}

		m_lastAcquire = acquireTime;
	}
}

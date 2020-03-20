#include "PulseWidthModulator.h"

using namespace IoT::UAV;

PulseWidthModulator::PulseWidthModulator(const float outputFrequency)
	: StreamNode<PulseWidthSet>([this](auto& dataInfo) { this->run(dataInfo); })
	, m_logger(Core::ILogger::getDefault())
	, m_updateDelay(1e6f / outputFrequency)
	, m_pulseWidthModulator(450)
{ }

bool PulseWidthModulator::initialize()
{
	if (!m_pulseWidthModulator.initialize()) {
		m_logger->log(Core::Error, "InputDevice", "Pulse width modulator initialize failed");
		return false;
	}

	return true;
}

void PulseWidthModulator::run(Core::DataInfo<PulseWidthSet>& dataInfo)
{
	const auto currentDelay = ellapsed();
	if (currentDelay.getMicros() >= m_updateDelay) {

		update(dataInfo);

		if (currentDelay.getMicros() > m_updateDelay * 2) {
			m_logger->log(Core::Warning, "InputDevice", "Pulse width modulator delay was " +
				std::to_string(currentDelay.getSecs<float>()) + "s. This is " +
				std::to_string(currentDelay.getMicros() / m_updateDelay) +
				" times greater than the optimal!");
		}
		dataInfo.m_propagate = true;
	}
	else
		dataInfo.m_propagate = false;
}

void PulseWidthModulator::update(Core::DataInfo<PulseWidthSet>& dataInfo)
{
	const PulseWidthSet& newPWSet = dataInfo.getSource<PulseWidthSet>().current();
	PulseWidthSet& currentPWSet = m_dataInfo.m_data;

	for (uint8_t i = newPWSet.getFirst(); i <= newPWSet.getLast(); i++) {
		const float newPW = newPWSet.getPW(i);
		const float currentPW = currentPWSet.getPW(i);

		if (newPW >= 0 && newPW != currentPW) {
			m_pulseWidthModulator.setValue(i, newPW);
			currentPWSet.setPW(i, newPW);
		}
	}

	dataInfo.m_propagate = true;
}

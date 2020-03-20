#include "PulseWidthSet.h"

using namespace IoT::UAV;

PulseWidthSet::PulseWidthSet(const PulseWidthSet &pulseWidthSet) {
	memcpy(m_pwmData, pulseWidthSet.m_pwmData, sizeof(m_pwmData));
	m_firstChannel = pulseWidthSet.m_firstChannel;
	m_lastChannel = pulseWidthSet.m_lastChannel;
}

void PulseWidthSet::setBounds(int ch) {
	m_firstChannel = std::min(m_firstChannel, (uint8_t) ch);
	m_lastChannel = std::max(m_lastChannel, (uint8_t) ch);
}

void PulseWidthSet::clear() {
	m_firstChannel = 15;
	m_lastChannel = 0;
	for (float &i : m_pwmData) {
		i = -1;
	}
}

float PulseWidthSet::getPW(uint8_t ch) const
{
	return m_pwmData[ch];
}

void PulseWidthSet::setPW(uint8_t ch, float value)
{
	m_pwmData[ch] = value;
	setBounds(ch);
}

void PulseWidthSet::setHSV(int wing, float h, float s, float v) {
	Core::ColorConvert::hsv2rgb(
			h, s, v,

			// Red
			&m_pwmData[4 + 0 * 4 + wing],
			// Green
			&m_pwmData[4 + 1 * 4 + wing],
			// Blue
			&m_pwmData[4 + 2 * 4 + wing]
	);
	setBounds(4 + 0 * 4 + wing);
	setBounds(4 + 1 * 4 + wing);
	setBounds(4 + 2 * 4 + wing);
}

void PulseWidthSet::setRGB(int wing, float r, float g, float b) {
	// Red
	if (r >= 0) {
		m_pwmData[4 + 0 * 4 + wing] = r * 2;
		setBounds(4 + 0 * 4 + wing);
	}

	// Green
	if (g >= 0) {
		m_pwmData[4 + 1 * 4 + wing] = g * 2;
		setBounds(4 + 1 * 4 + wing);
	}

	// Blue
	if (b >= 0) {
		m_pwmData[4 + 2 * 4 + wing] = b * 2;
		setBounds(4 + 2 * 4 + wing);
	}
}

void PulseWidthSet::setRGB(float r, float g, float b) {
	for (int i = 0; i < 4; i++) {
		// Red
		if (r >= 0) {
			m_pwmData[4 + 0 * 4 + i] = r * 2;
			setBounds(4 + 0 * 4 + i);
		}
		// Green
		if (g >= 0) {
			m_pwmData[4 + 1 * 4 + i] = g * 2;
			setBounds(4 + 1 * 4 + i);

		}
		// Blue
		if (b >= 0) {
			m_pwmData[4 + 2 * 4 + i] = b * 2;
			setBounds(4 + 2 * 4 + i);
		}
	}
}

float PulseWidthSet::getESCValue(int wing) const
{
	return m_pwmData[wing] - 1.0f;
}

void PulseWidthSet::setESCValue(int wing, float value) {
	m_pwmData[wing] = 1.0f + value;
	setBounds(wing);
}

#pragma once

#include <cmath>

namespace IoT {
	namespace Device {

		struct BarometerData {
			float getAltitude(float sealevelPressure = 101325.0f) const {
				return 44330.0f * (1.0f - powf(m_pressure / sealevelPressure, (1.0f / 5.255f)));
			}

			float getSealevel(float altitude = 0.0f) const {
				return m_pressure / powf(1.0f - altitude / 44330.0f, 5.255f);
			}

			float getTemperature() const {
				return m_temperature;
			}

			void setTemperature(float temperature) {
				m_temperature = temperature;
			}

			float getPressure() const {
				return m_pressure;
			}

			void setPressure(float pressure) {
				m_pressure = pressure;
			}

		private:
			float m_temperature = 0;
			float m_pressure = 0;
		};
	}
}

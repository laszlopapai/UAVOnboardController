#pragma once


#include <algorithm>
#include <stdio.h>
#include <string.h>
#include <Math/ColorConvert.h>

namespace IoT {
	namespace UAV {

		class PulseWidthSet {
			float m_pwmData[16]{
					-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1
			};
			uint8_t m_firstChannel = 15;
			uint8_t m_lastChannel = 0;

			void setBounds(int ch);

		public:
			PulseWidthSet() = default;
			PulseWidthSet(const PulseWidthSet &pulseWidthSet);

			uint8_t getFirst() const {
				return m_firstChannel;
			}

			uint8_t getLast() const {
				return m_lastChannel;
			}

			void clear();

			float getPW(uint8_t ch) const;

			void setPW(uint8_t ch, float value);

			void setHSV(int wing, float h, float s, float v);

			void setRGB(int wing, float r, float g, float b);

			void setRGB(float r, float g, float b);

			float getESCValue(int channel) const;

			void setESCValue(int channel, float value);
		};
	}
}

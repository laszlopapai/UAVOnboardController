#pragma once

#include <cfloat>
#include <algorithm>
#include <cstdint>

namespace IoT {
	namespace Core {

		class PIDController
		{

		public:
			PIDController(float proportionalGain, float integralGain, float derivativeGain, float maxIntegralSum);

			float calculate(float setPoint, float processVariable, float dt);

		private:
			const float m_proportionalGain;
			const float m_integralGain;
			const float m_derivativeGain;
			const float m_maxIntegralSum;
			float m_previousError;
			float m_integral;
		};
	}
}

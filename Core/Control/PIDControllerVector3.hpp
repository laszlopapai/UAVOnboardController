#pragma once

#include "PIDController.h"
#include "Math/Vector3.h"

namespace IoT {
	namespace Core {

		class PIDControllerVector3 {
		public:
			PIDControllerVector3(float proportionalGain, float integralGain, float derivativeGain, float maxIntegralSum)
				: m_pidControllerX(proportionalGain, integralGain, derivativeGain, maxIntegralSum)
				, m_pidControllerY(proportionalGain, integralGain, derivativeGain, maxIntegralSum)
				, m_pidControllerZ(proportionalGain, integralGain, derivativeGain, maxIntegralSum) {}

			Vector3 &calculate(const Vector3 &setPoint, Vector3 &processVariable, float dt) {
				processVariable.x = m_pidControllerX.calculate(setPoint.x, processVariable.x, dt);
				processVariable.y = m_pidControllerY.calculate(setPoint.y, processVariable.y, dt);
				processVariable.z = m_pidControllerZ.calculate(setPoint.z, processVariable.z, dt);

				return processVariable;
			}

		private:
			PIDController m_pidControllerX;
			PIDController m_pidControllerY;
			PIDController m_pidControllerZ;
		};
	}
}

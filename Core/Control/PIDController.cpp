#include "PIDController.h"

using namespace IoT::Core;

PIDController::PIDController(const float proportionalGain,
							 const float integralGain,
							 const float derivativeGain,
							 const float maxIntegralSum)
	: m_proportionalGain(proportionalGain)
	, m_integralGain(integralGain)
	, m_derivativeGain(derivativeGain)
	, m_maxIntegralSum(maxIntegralSum)
	, m_previousError(0)
	, m_integral(0)
{ }

float PIDController::calculate(const float setPoint, const float processVariable, const float dt)
{
	// Calculate error
	const float error = setPoint - processVariable;

	// Proportional term
	const float pOut = m_proportionalGain * error;

	// Integral term
	m_integral += error * dt;
	m_integral = std::min(std::max(m_integral, -m_maxIntegralSum), m_maxIntegralSum);
	const float iOut = m_integralGain * m_integral;

	// Derivative term
	const float derivative = (error - m_previousError) / dt;
	const float dOut = m_derivativeGain * derivative;

	// Save error to previous error
	m_previousError = error;

	// Calculate total output
	return  pOut + iOut + dOut;
}
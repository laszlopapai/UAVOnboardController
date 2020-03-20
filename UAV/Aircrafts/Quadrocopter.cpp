#include "Quadrocopter.h"
#include <math.h>

using namespace IoT::UAV;

void Quadrocopter::configure(const AngularControlTrust& angularControlTrust, PulseWidthSet& pwm)
{
	pwm.clear();
	
	Wings wing;
	if (angularControlTrust.angles.z < -.25f)
		if (angularControlTrust.angles.y < -.25f)
			wing = Wings::RearLeft;
		else if (angularControlTrust.angles.y > .25f)
			wing = Wings::FrontLeft;
		else
			return;
	else if (angularControlTrust.angles.z > .25f)
		if (angularControlTrust.angles.y < -.25f)
			wing = Wings::RearRight;
		else if (angularControlTrust.angles.y > .25f)
			wing = Wings::FrontRight;
		else
			return;
	else
		return;

	if (angularControlTrust.thrust > .75f || angularControlTrust.thrust < .25f)
		pwm.setESCValue(wing, angularControlTrust.thrust > .5f ? 1.0f : 0.0f);
}

void Quadrocopter::setAirflow(const AngularControlTrust& angularControlTrust, PulseWidthSet& dataValue)
{
	auto yaw = clamp(angularControlTrust.angles.z, -1, 1);
	auto pitch = clamp(angularControlTrust.angles.y, -1, 1);
	auto roll = clamp(angularControlTrust.angles.x, -1, 1);
	auto thrust = angularControlTrust.thrust;

	float effectivePitch = pitch;
	float effectiveRoll = roll;

	float effectiveThrust = thrust *
		(1.0f / (1.0f - fabsf(pitch) / 2.0f)) *
		(1.0f / (1.0f - fabsf(roll) / 2.0f)) *
		(1.0f / (1.0f - fabsf(yaw) / 2.0f));

	float effectiveYaw;

	if (effectiveThrust <= 1.0f) {
		effectiveYaw = yaw;

		return setStableAirflow(effectiveThrust, effectiveYaw, effectivePitch, effectiveRoll, dataValue);
	}

	effectiveThrust = 1.0f;

	float requestedThrustForYaw = thrust * (1.0f / (1.0f - std::abs(yaw) / 2.0f)) - thrust;
	float availableThrustForYaw = 1.0f - (thrust * (1.0f / (1.0f - std::abs(pitch) / 2.0f)) *
		(1.0f / (1.0f - std::abs(roll) / 2.0f)));
	effectiveYaw =
		availableThrustForYaw > 0.0f ? yaw * availableThrustForYaw / requestedThrustForYaw : 0.0f;

	setStableAirflow(effectiveThrust, effectiveYaw, effectivePitch, effectiveRoll, dataValue);
}

void Quadrocopter::setStableAirflow(float thrust, float yaw, float pitch, float roll, PulseWidthSet& pwm) const
{

	float rollLeft   = clamp(roll + 1.0f);
	float rollRight  = clamp(1.0f - roll);

	float pitchFront = clamp(pitch + 1.0f);
	float pitchRear  = clamp(1.0f - pitch);

	float yawCW  = clamp(yaw + 1.0f);
	float yawCCW = clamp(1.0f - yaw);

	float fl = thrust * rollLeft  * pitchFront * yawCCW;
	float fr = thrust * rollRight * pitchFront * yawCW;
	float rl = thrust * rollLeft  * pitchRear  * yawCW;
	float rr = thrust * rollRight * pitchRear  * yawCCW;

	float min = thrust > 0.01f ? m_minimumSpeed : 0;
	fl = std::max(fl, min);
	fr = std::max(fr, min);
	rl = std::max(rl, min);
	rr = std::max(rr, min);

	pwm.setESCValue(Quadrocopter::FrontLeft, fl);
	pwm.setESCValue(Quadrocopter::FrontRight, fr);
	pwm.setESCValue(Quadrocopter::RearLeft, rl);
	pwm.setESCValue(Quadrocopter::RearRight, rr);
}

void Quadrocopter::setIdle(PulseWidthSet& pwm) {
	pwm.setPW(Quadrocopter::FrontLeft, 0);
	pwm.setPW(Quadrocopter::FrontRight, 0);
	pwm.setPW(Quadrocopter::RearLeft, 0);
	pwm.setPW(Quadrocopter::RearRight, 0);
}

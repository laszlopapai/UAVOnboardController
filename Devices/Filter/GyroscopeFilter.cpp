#include "GyroscopeFilter.h"
#include <Math/Quaternion.h>

using namespace IoT::Device;

GyroscopeFilter::GyroscopeFilter()
{
	loadCalibration("gyroscope_calibration.dat", (char*)this + sizeof(CalibratedFilter),
												 sizeof(GyroscopeFilter) - sizeof(CalibratedFilter));
}

bool GyroscopeFilter::calibrate(Core::Vector3 angularVelocity, uint32_t deltaTime) {
	if (m_timeout > 0) {
		m_timeout -= deltaTime;

		m_max.x = std::max(m_max.x, angularVelocity.x);
		m_max.y = std::max(m_max.y, angularVelocity.y);
		m_max.z = std::max(m_max.z, angularVelocity.z);

		m_min.x = std::min(m_min.x, angularVelocity.x);
		m_min.y = std::min(m_min.y, angularVelocity.y);
		m_min.z = std::min(m_min.z, angularVelocity.z);

		m_meanCount++;
		float ratio = 1.0f / m_meanCount;

		m_mean *= (1 - ratio);
		m_mean += angularVelocity * ratio;
	}

	if (isCalibrated()) {
		saveCalibration("gyroscope_calibration.dat", (char*)this + sizeof(CalibratedFilter),
													 sizeof(GyroscopeFilter) - sizeof(CalibratedFilter));
		return false;
	}

	return true;
}

void GyroscopeFilter::filter(Core::Vector3 & angularVelocity) {
	angularVelocity.x -= m_mean.x;
	angularVelocity.y -= m_mean.y;
	angularVelocity.z -= m_mean.z;

	// Axis swap to NASA standard
	std::swap(angularVelocity.x, angularVelocity.y);
	angularVelocity.y *= -1;
}

bool GyroscopeFilter::isCalibrating()
{
	return m_timeout < GyroscopeCalibrationTime && !isCalibrated();
}

bool GyroscopeFilter::isCalibrated() {
	return m_timeout <= 0;
}

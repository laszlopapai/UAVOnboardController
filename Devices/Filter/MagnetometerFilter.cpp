#include "MagnetometerFilter.h"

using namespace IoT::Core;
using namespace IoT::Device;

MagnetometerFilter::MagnetometerFilter()
{
	loadCalibration("magnetometer_calibration.dat", (char*)this + sizeof(CalibratedFilter),
													sizeof(MagnetometerFilter) - sizeof(CalibratedFilter));
}

void MagnetometerFilter::filter(Vector3 & magnetism) {
	magnetism.x = 2.0f * (magnetism.x - m_min.x) /
						 (m_max.x - m_min.x) - 1.0f;
	magnetism.y = 2.0f * (magnetism.y - m_min.y) /
						 (m_max.y - m_min.y) - 1.0f;
	magnetism.z = 2.0f * (magnetism.z - m_min.z) /
						 (m_max.z - m_min.z) - 1.0f;


	// Axis swap to NASA standard
	std::swap(magnetism.x, magnetism.z);
	std::swap(magnetism.y, magnetism.z);
	magnetism.y *= -1;
}

bool MagnetometerFilter::calibrate(Vector3 magnetism, uint32_t deltaTime) {
	m_max.x = std::max(m_max.x, magnetism.x);
	m_max.y = std::max(m_max.y, magnetism.y);
	m_max.z = std::max(m_max.z, magnetism.z);

	m_min.x = std::min(m_min.x, magnetism.x);
	m_min.y = std::min(m_min.y, magnetism.y);
	m_min.z = std::min(m_min.z, magnetism.z);

	m_timeout -= deltaTime;
	if (isCalibrated()) {
		saveCalibration("magnetometer_calibration.dat", (char*)this + sizeof(CalibratedFilter),
														sizeof(MagnetometerFilter) - sizeof(CalibratedFilter));
		return false;
	}
	return true;
}

bool MagnetometerFilter::isCalibrating()
{
	return m_timeout < MagnetometerCalibrationTime && !isCalibrated();
}

bool MagnetometerFilter::isCalibrated() {
	return m_timeout <= 0;
}

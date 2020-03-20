#include "AccelerometerFilter.h"

using namespace IoT::Core;
using namespace IoT::Device;

AccelerometerFilter::AccelerometerFilter()
{
	loadCalibration("accelerometer_calibration.dat", (char*)this + sizeof(CalibratedFilter),
													 sizeof(AccelerometerFilter) - sizeof(CalibratedFilter));
}

void AccelerometerFilter::filter(Vector3 & acceleration) {
	acceleration /= m_mean;


	// Axis swap to NASA standard
	std::swap(acceleration.x, acceleration.y);
	acceleration.x *= -1;
	acceleration.z *= -1;
}

bool AccelerometerFilter::calibrate(Vector3 acceleration, uint32_t deltaTime) {
	
	if (acceleration.length() <= std::numeric_limits<float>::epsilon())
		return true;
	
	// Axis swap to NASA standard
	std::swap(acceleration.x, acceleration.y);
	acceleration.x *= -1;
	acceleration.z *= -1;

	m_meanCount++;
	float ratio = 1.0f / m_meanCount;
	m_mean = m_mean * (1 - ratio) +
					   acceleration.length() * ratio;

	m_gravityVector = m_gravityVector * (1 - ratio) + acceleration / m_mean * ratio;

	m_timeout -= deltaTime;
	if (isCalibrated()) {
		m_gravityVector.normalise();

		saveCalibration("accelerometer_calibration.dat", (char*)this + sizeof(CalibratedFilter),
														 sizeof(AccelerometerFilter) - sizeof(CalibratedFilter));
		return false;
	}

	return true;
}

bool AccelerometerFilter::isCalibrating()
{
	return  m_timeout < AccelerometerCalibrationTime && !isCalibrated();
}

bool AccelerometerFilter::isCalibrated() {
	return m_timeout <= 0;
}

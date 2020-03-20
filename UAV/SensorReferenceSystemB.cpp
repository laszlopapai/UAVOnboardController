#include "SensorReferenceSystemB.h"
#include "Math/AngleConvert.h"
#include "Logger.h"

using namespace IoT::UAV;

SensorReferenceSystemB::SensorReferenceSystemB()
	: m_logger(Logger::getDefault())

	, m_angularVelocity([this](auto& dataInfo) { this->clearSignal(dataInfo, &m_gyroscopeFilter); })
	, m_acceleration([this](auto& dataInfo) { this->clearSignal(dataInfo, &m_accelerometerFilter); })
	, m_magneticNorthDirection([this](auto& dataInfo) { this->clearSignal(dataInfo, &m_magnetometerFilter); })

	, m_rotation([this](auto& dataInfo) { this->calcRotation(dataInfo); })
	, m_accelerometerOrientation([this](auto& dataInfo) { this->calcAccelerationOrientation(dataInfo); })

	, m_groundOrientation([this](auto& dataInfo) { this->combineGroundOrientation(dataInfo); })
	, m_magneticNorthOrientation([this](auto& dataInfo) { this->combineNorthOrientation(dataInfo); })

	, m_accelerationOverGround([this](auto& dataInfo) { this->combineAccelerationOverGround(dataInfo); })
	, m_angularPosition([this](auto& dataInfo) { this->combineAngularPosition(dataInfo); })
{
}

bool SensorReferenceSystemB::initialize()
{
	m_acceleration | m_accelerationOverGround;
	m_acceleration | m_accelerometerOrientation | m_accelerationOverGround;
	m_accelerometerOrientation | m_groundOrientation | m_accelerationOverGround;


	m_angularVelocity | m_rotation | m_groundOrientation | m_angularPosition;
	m_rotation | m_magneticNorthOrientation;

	m_magneticNorthDirection | m_magneticNorthOrientation | m_angularPosition;

	return true;
}

void SensorReferenceSystemB::clearSignal(Core::DataInfo<Core::Vector3>& dataInfo,
	Device::CalibratedFilter<Core::Vector3>* filter)
{
	auto& source = dataInfo.getSource<Core::Vector3>();
	source >> dataInfo.m_data;
	assert(!dataInfo.m_data.isNaN());

	dataInfo.m_propagate = filter->isCalibrated();
	if (filter->isCalibrated())
		filter->filter(dataInfo.m_data);
	else {
		if (!filter->isCalibrating())
			m_logger->log(Core::Info, "Calibration", "Gyroscope calibration started.");
		filter->calibrate(dataInfo.m_data, source.interval().getMicros());
	}
}

void SensorReferenceSystemB::calcRotation(Core::DataInfo<Core::Quaternion>& dataInfo) const
{
	auto angularVelocity = m_angularVelocity.current();
	assert(!angularVelocity.isNaN());
	auto dt = m_rotation.interval().getSecs<float>();

	auto magnitude = angularVelocity.normalise();
	auto theta = Core::AngleConvert::ToRadius * magnitude * dt;

	dataInfo.m_data.fromAngleAxis(-theta, angularVelocity);
	dataInfo.m_propagate = true;
}

void SensorReferenceSystemB::calcAccelerationOrientation(
	Core::DataInfo<Core::Quaternion>& dataInfo) const
{
	static Core::Quaternion calibration;
	static bool isCalibrated = false;

	auto acceleration = m_acceleration.current();
	assert(!acceleration.isNaN());
	if (acceleration.length() < 0.2f) {
		return;
	}
	acceleration.normalise();

	auto accelerometerOrientation = Core::Quaternion(1.0f, acceleration);
	accelerometerOrientation.normalise();

	if (!isCalibrated) {
		isCalibrated = true;

		/*auto v = eulerFromVector(accelerometerOrientation.getAxis());
		const auto theta = v.normalise();

		calibration.fromAngleAxis(theta, v);*/
	}
	
	dataInfo.m_data = calibration * accelerometerOrientation * calibration.inverse();
	dataInfo.m_propagate = true;
}

void SensorReferenceSystemB::combineGroundOrientation(
	Core::DataInfo<Core::Quaternion>& dataInfo)
{
	if (dataInfo.m_source == &m_rotation && m_accelerometerOrientation.isReady()) {
		assert(!m_rotation.current().isNaN());

		dataInfo.m_data =
			(m_rotation.current() *
				dataInfo.m_data *
				m_rotation.current().inverse())
			.slerpAngle(0.00025f, m_accelerometerOrientation.current());
		dataInfo.m_propagate = true;
	}
}

void SensorReferenceSystemB::combineNorthOrientation(
	Core::DataInfo<Core::Quaternion>& dataInfo)
{
	static Core::Quaternion magnometerOrientation;
	Core::Quaternion& magneticNorthOrientation = dataInfo.m_data;

	if (dataInfo.m_source == &m_magneticNorthDirection) {
		auto direction = m_magneticNorthDirection.current();

		if (direction.isNaN()) {
			assert(false);
		}

		direction.normalise();
		magnometerOrientation = Core::Quaternion(1, direction);
		magnometerOrientation.normalise();
	}

	if (dataInfo.m_source == &m_rotation && m_magneticNorthDirection.isReady()) {
		if (m_rotation.current().isNaN()) {
			assert(false);
		}

		dataInfo.m_data = 
			(m_rotation.current() * 
			dataInfo.m_data *
			m_rotation.current().inverse())
		.slerpAngle(0.01f, magnometerOrientation);
		dataInfo.m_propagate = true;
	}
}

void SensorReferenceSystemB::combineAccelerationOverGround(
	Core::DataInfo<Core::Vector3>& dataInfo)
{
}

void SensorReferenceSystemB::combineAngularPosition(
	Core::DataInfo<Core::Vector3>& dataInfo) const
{
	if (!m_groundOrientation.isReady() || !m_magneticNorthOrientation.isReady()) {
		return;
	}

	assert(!m_groundOrientation.current().isNaN());	
	assert(!m_magneticNorthOrientation.current().isNaN());
	
	auto down = m_groundOrientation.current().getAxis();
	auto magneticNorth = m_magneticNorthOrientation.current().getAxis();
	auto east = down.cross(magneticNorth);
	east.normalise();
	auto northAlongSurface = east.cross(down);
	northAlongSurface.normalise();

	auto& angularPosition = dataInfo.m_data;
	down.normalise();
	angularPosition = down.eulerFromDirection(Core::Vector3::Down);

	//Core::Vector3 axis{northAlongSurface.x, -northAlongSurface.z, northAlongSurface.y};
	angularPosition.z = 0;//eulerFromVector(axis).y; //yaw

	assert(!angularPosition.isNaN());
	dataInfo.m_propagate = true;
}

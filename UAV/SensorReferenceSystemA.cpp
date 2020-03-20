#include "SensorReferenceSystemA.h"
#include "Math/AngleConvert.h"
#include "Logger.h"

using namespace IoT::UAV;

SensorReferenceSystemA::SensorReferenceSystemA(const float k)
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

bool SensorReferenceSystemA::initialize()
{
	m_acceleration | m_accelerationOverGround;
	m_acceleration | m_accelerometerOrientation | m_accelerationOverGround;
	m_accelerometerOrientation | m_groundOrientation | m_accelerationOverGround;


	m_angularVelocity | m_rotation | m_groundOrientation | m_angularPosition;
	m_rotation | m_magneticNorthOrientation;

	m_magneticNorthDirection | m_magneticNorthOrientation | m_angularPosition;

	return true;
}

void SensorReferenceSystemA::clearSignal(Core::DataInfo<Core::Vector3>& dataInfo,
	Device::CalibratedFilter<Core::Vector3>* filter)
{
	auto& source = dataInfo.getSource<Core::Vector3>();
	dataInfo.m_data = source.current();
	dataInfo.m_propagate = filter->isCalibrated();
	if (filter->isCalibrated())
		filter->filter(dataInfo.m_data);
	else {
		if (!filter->isCalibrating())
			m_logger->log(Core::Info, "Calibration", "Gyroscope calibration started.");
		filter->calibrate(dataInfo.m_data, source.interval().getMicros());
	}
}

void SensorReferenceSystemA::calcRotation(Core::DataInfo<Core::Quaternion>& dataInfo) const
{
	auto angularVelocity = m_angularVelocity.current();
	auto dt = m_rotation.interval().getSecs<float>();

	auto magnitude = angularVelocity.normalise();
	auto theta = Core::AngleConvert::ToRadius * magnitude * dt;

	dataInfo.m_data.fromAngleAxis(-theta, angularVelocity);
}

void SensorReferenceSystemA::calcAccelerationOrientation(
	Core::DataInfo<Core::Quaternion>& dataInfo) const
{
	static Core::Quaternion calibration;
	static bool isCalibrated = false;

	auto acceleration = m_acceleration.current();
	if (acceleration.length() < 0.2f) {
		dataInfo.m_propagate = false;
		return;
	}
	acceleration.normalise();

	auto accelerometerOrientation = Core::Quaternion(1.0f, acceleration);
	accelerometerOrientation.normalise();

	if (!isCalibrated) {
		isCalibrated = true;

		auto v = eulerFromVector(accelerometerOrientation.getAxis());
		const auto theta = v.normalise();

		calibration.fromAngleAxis(theta, v);
	}
	
	dataInfo.m_data = calibration * accelerometerOrientation * calibration.inverse();
}

void SensorReferenceSystemA::combineGroundOrientation(
	Core::DataInfo<Core::Quaternion>& dataInfo)
{
	if (dataInfo.m_source == &m_rotation) {
		dataInfo.m_data =
			(m_rotation.current() *
				dataInfo.m_data *
				m_rotation.current().inverse())
			.slerpAngle(0.00025f, m_accelerometerOrientation.current());
	}
}

void SensorReferenceSystemA::combineNorthOrientation(
	Core::DataInfo<Core::Quaternion>& dataInfo)
{
	static Core::Quaternion magnometerOrientation;
	Core::Quaternion& magneticNorthOrientation = dataInfo.m_data;

	if (dataInfo.m_source == &m_magneticNorthDirection) {
		auto direction = m_magneticNorthDirection.current();
		direction.normalise();
		magnometerOrientation = Core::Quaternion(1, direction);
		magnometerOrientation.normalise();
	}

	if (dataInfo.m_source == &m_rotation) {
		magneticNorthOrientation = 
			(m_rotation.current() * 
			magneticNorthOrientation * 
			m_rotation.current().inverse())
		.slerpAngle(0.01f, magnometerOrientation);
	}
}

void SensorReferenceSystemA::combineAccelerationOverGround(
	Core::DataInfo<Core::Vector3>& dataInfo)
{
}

void SensorReferenceSystemA::combineAngularPosition(
	Core::DataInfo<Core::Vector3>& dataInfo) const
{
	auto down = m_groundOrientation.current().getAxis();
	auto magneticNorth = m_magneticNorthOrientation.current().getAxis();
	auto east = down.cross(magneticNorth);
	east.normalise();
	auto northAlongSurface = east.cross(down);
	northAlongSurface.normalise();

	auto& angularPosition = dataInfo.m_data;
	angularPosition = eulerFromVector(down);

	Core::Vector3 axis{northAlongSurface.x, -northAlongSurface.z, northAlongSurface.y};
	angularPosition.z = eulerFromVector(axis).y; //yaw
}

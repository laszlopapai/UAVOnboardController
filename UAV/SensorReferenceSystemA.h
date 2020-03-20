#pragma once

#include <Math/Vector3.h>
#include "Data/StreamNode.h"
#include "Filter/GyroscopeFilter.h"
#include "Filter/AccelerometerFilter.h"
#include "Filter/MagnetometerFilter.h"
#include <ILogger.h>
#include "Filter/C2ComplementerFilter.h"

namespace IoT {
	namespace UAV {

		class SensorReferenceSystemA
		{

		public:
			SensorReferenceSystemA(const float k);

			bool initialize();

			Core::StreamNode<Core::Vector3>& getAngularVelocity()
			{
				return m_angularVelocity;
			}

			Core::StreamNode<Core::Vector3>& getAngularPosition()
			{
				return m_angularPosition;
			}

			Core::StreamNode<Core::Vector3>& getAccelerationIn()
			{
				return m_acceleration;
			}

			Core::StreamNode<Core::Vector3>& getMagnetismIn()
			{
				return m_magneticNorthDirection;
			}

		private:
			void clearSignal(Core::DataInfo<Core::Vector3>& dataInfo, Device::CalibratedFilter<Core::Vector3>* filter);

		private:
			Core::ILogger *m_logger;

			Device::GyroscopeFilter m_gyroscopeFilter;
			Device::AccelerometerFilter m_accelerometerFilter;
			Device::MagnetometerFilter m_magnetometerFilter;

			Device::C2ComplementerFilter m_groundAngleX;
			Device::C2ComplementerFilter m_groundAngleY;
			Device::C2ComplementerFilter m_northAngleY;
			Device::C2ComplementerFilter m_northAngleZ;

			Core::StreamNode<Core::Vector3> m_angularVelocity;
			Core::StreamNode<Core::Vector3> m_acceleration;
			Core::StreamNode<Core::Vector3> m_magneticNorthDirection;

			Core::StreamNode<Core::Vector3> m_accelerationOverGround;
			Core::StreamNode<Core::Vector3> m_angularPosition;

		};
	}
}

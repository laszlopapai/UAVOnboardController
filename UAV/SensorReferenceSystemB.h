#pragma once

#include <Math/Vector3.h>
#include <Math/Quaternion.h>
#include "Data/StreamNode.h"
#include "Filter/GyroscopeFilter.h"
#include "Filter/AccelerometerFilter.h"
#include "Filter/MagnetometerFilter.h"
#include <ILogger.h>

namespace IoT {
	namespace UAV {

		class SensorReferenceSystemB
		{

		public:
			SensorReferenceSystemB();

			bool initialize();

			Core::StreamNode<Core::Vector3>& getAngularVelocity()
			{
				return m_angularVelocity;
			}

			Core::StreamNode<Core::Vector3>& getAngularPositionOut()
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

			void calcRotation(Core::DataInfo<Core::Quaternion>& dataInfo) const;
			void calcAccelerationOrientation(Core::DataInfo<Core::Quaternion>& dataInfo) const;

			void combineGroundOrientation(Core::DataInfo<Core::Quaternion>& dataInfo);
			void combineNorthOrientation(Core::DataInfo<Core::Quaternion>& dataInfo);

			void combineAccelerationOverGround(Core::DataInfo<Core::Vector3>& dataInfo);
			void combineAngularPosition(Core::DataInfo<Core::Vector3>& dataInfo) const;


		private:
			Core::ILogger *m_logger;

			Device::GyroscopeFilter m_gyroscopeFilter;
			Device::AccelerometerFilter m_accelerometerFilter;
			Device::MagnetometerFilter m_magnetometerFilter;

			Core::StreamNode<Core::Vector3> m_angularVelocity;
			Core::StreamNode<Core::Vector3> m_acceleration;
			Core::StreamNode<Core::Vector3> m_magneticNorthDirection;

			Core::StreamNode<Core::Quaternion> m_rotation;
			Core::StreamNode<Core::Quaternion> m_accelerometerOrientation;

			Core::StreamNode<Core::Quaternion> m_groundOrientation;
			Core::StreamNode<Core::Quaternion> m_magneticNorthOrientation;

			Core::StreamNode<Core::Vector3> m_accelerationOverGround;
			Core::StreamNode<Core::Vector3> m_angularPosition;

		};
	}
}

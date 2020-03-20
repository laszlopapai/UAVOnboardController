#pragma once

#include "Data/StreamNode.h"
#include "Math/Vector3.h"
#include "Control/PIDControllerVector3.hpp"
#include <Data/StreamBinder.h>
#include "Structures/AngularControlTrust.h"
#include "Structures/RemoteIn.h"

namespace IoT {
	namespace UAV {

		class Pilot
		{
			enum StreamTag : uint8_t
			{
				AngularPosition = 0,
				AngularVelocity = 1,
				AircraftControl = 2
			};


		public:
			Pilot();

			bool initialize();
				
			Core::StreamPart& getAngularPositionIn()
			{
				return m_angularPosition;
			}

			Core::StreamPart& getAngularVelocityIn()
			{
				return m_angularVelocity;
			}

			Core::StreamPart& getRemoteIn()
			{
				return m_remoteIn;
			}

			Core::StreamNode<AngularControlTrust>& getAngularControlTrustOut()
			{
				return m_angularVelocityControlScale;
			}


		private:
			void angularPositionPID(Core::DataInfo<Core::Vector3>& dataInfo);

			void angularVelocitySetpointScale(Core::DataInfo<Core::Vector3>& dataInfo) const;

			void angularVelocityControlPID(Core::DataInfo<Core::Vector3>& dataInfo);

			void angularVelocityControlScale(Core::DataInfo<AngularControlTrust>& dataInfo) const;

		private:
			Core::StreamNode<Core::Vector3> m_angularPosition;
			Core::StreamNode<Core::Vector3> m_angularVelocity;
			Core::StreamNode<RemoteIn> m_remoteIn;

			Core::Vector3 m_angularPositionStableScale;
			Core::PIDControllerVector3 m_angularPositionStablePID;

			Core::Vector3 m_angularPositionRateScale;
			Core::PIDControllerVector3 m_angularPositionRatePID;

			// Data cache

			Core::Vector3 m_lastAngularVelocitySetpoint;
			Core::Vector3 m_lastAngularVelocity;

			// Streams
			Core::StreamNode<Core::Vector3> m_angularPositionControlPID;
			Core::StreamNode<Core::Vector3> m_angularVelocitySetpointScale;
			Core::StreamNode<Core::Vector3> m_angularVelocityControlPID;
			Core::StreamNode<AngularControlTrust> m_angularVelocityControlScale;
		};

	}
}

#pragma once

#include "KalmanFilter.h"
#include <Thread/IFilter.h>
#include <Math/Vector3.h>

namespace IoT {
	namespace Device {

		class KalmanFilterVector3
			: public Core::IFilter<Core::Vector3>
		{

		public:
			KalmanFilterVector3(float estimatedMeasurementNoiseCovariance,
								float estimatedProcessNoiseCovariance,
								float controlWeight)
				: m_kalmanFilterX(estimatedMeasurementNoiseCovariance, estimatedProcessNoiseCovariance, controlWeight)
				, m_kalmanFilterY(estimatedMeasurementNoiseCovariance, estimatedProcessNoiseCovariance, controlWeight)
				, m_kalmanFilterZ(estimatedMeasurementNoiseCovariance, estimatedProcessNoiseCovariance, controlWeight)
			{}

			KalmanFilterVector3(float estimatedMeasurementNoiseCovariance,
								float estimatedProcessNoiseCovariance,
								float controlWeight,
								float initialEstimate,
								float initialErrorCovariance)
				: m_kalmanFilterX(estimatedMeasurementNoiseCovariance, estimatedProcessNoiseCovariance,
									controlWeight, initialEstimate, initialErrorCovariance)
				, m_kalmanFilterY(estimatedMeasurementNoiseCovariance, estimatedProcessNoiseCovariance,
									controlWeight, initialEstimate, initialErrorCovariance)
				, m_kalmanFilterZ(estimatedMeasurementNoiseCovariance, estimatedProcessNoiseCovariance,
									controlWeight, initialEstimate, initialErrorCovariance)
			{}

			void filter(Core::Vector3 &vector) override
			{
				vector.x = m_kalmanFilterX.filter(vector.x);
				vector.y = m_kalmanFilterY.filter(vector.y);
				vector.z = m_kalmanFilterZ.filter(vector.z);
			}

		private:
			KalmanFilter m_kalmanFilterX;
			KalmanFilter m_kalmanFilterY;
			KalmanFilter m_kalmanFilterZ;
		};
	}
}

#pragma once

#include <cmath>

namespace IoT {
	namespace Device {

		class KalmanFilter {
			float m_estimatedMeasurementNoiseCovariance;
			float m_estimatedProcessNoiseCovariance;
			float m_controlWeight;
			float m_priorEstimate;
			float m_priorErrorCovariance;

		public:
			KalmanFilter(float estimatedMeasurementNoiseCovariance, float estimatedProcessNoiseCovariance,
							float controlWeight);

			KalmanFilter(float estimatedMeasurementNoiseCovariance, float estimatedProcessNoiseCovariance,
							float controlWeight, float initialEstimate, float initialErrorCovariance);

			float filter(float measurement);

			float filter(float control, float measurement);

		private:
			static float
			predictEstimate(float priorWeight, float priorEstimate, float controlWeight, float control);

			static float predictErrorCovariance(float priorWeight, float priorErrorCovariance,
												float estimatedProcessNoiseCovariance);

			static float calculateGain(float errorCovariance, float estimatedMeasurementNoiseCovariance);

			static float correctEstimate(float estimate, float gain, float measurement);

			static float correctErrorCovariance(float gain, float errorCovariance);
		};
	}
}

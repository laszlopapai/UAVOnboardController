#include "KalmanFilter.h"

using namespace IoT::Device;

KalmanFilter::KalmanFilter(const float estimatedMeasurementNoiseCovariance,
						   const float estimatedProcessNoiseCovariance,
						   const float controlWeight)
	: m_estimatedMeasurementNoiseCovariance(estimatedMeasurementNoiseCovariance)
	, m_estimatedProcessNoiseCovariance(estimatedProcessNoiseCovariance)
	, m_controlWeight(controlWeight)
	, m_priorEstimate(0)
	, m_priorErrorCovariance(1.0f)
{ }

KalmanFilter::KalmanFilter(const float estimatedMeasurementNoiseCovariance,
						   const float estimatedProcessNoiseCovariance,
						   const float controlWeight,
						   const float initialEstimate,
						   const float initialErrorCovariance)
	: m_estimatedMeasurementNoiseCovariance(estimatedMeasurementNoiseCovariance)
	, m_estimatedProcessNoiseCovariance(estimatedProcessNoiseCovariance)
	, m_controlWeight(controlWeight)
	, m_priorEstimate(initialEstimate)
	, m_priorErrorCovariance(initialErrorCovariance)
{ }

float KalmanFilter::filter(const float measurement) {
	return filter(measurement, measurement);
}

float KalmanFilter::filter(const float control, const float measurement)
{
	// prediction
    const float estimate = predictEstimate(1.0f - m_controlWeight, m_priorEstimate, m_controlWeight, control);
    const float errorCovariance = predictErrorCovariance(1.0f - m_controlWeight, m_priorErrorCovariance,
														m_estimatedProcessNoiseCovariance);

	// correction
    const float gain = calculateGain(errorCovariance, m_estimatedMeasurementNoiseCovariance);
    const float estimateCorrected = correctEstimate(estimate, gain, measurement);
    const float errorCovarianceCorrected = correctErrorCovariance(gain, errorCovariance);

	m_priorErrorCovariance = errorCovarianceCorrected;
	return m_priorEstimate = estimateCorrected;
}

float KalmanFilter::predictEstimate(const float priorWeight, const float priorEstimate, const float controlWeight, const float control) {
	return priorWeight * priorEstimate + controlWeight * control;
}

float KalmanFilter::predictErrorCovariance(const float priorWeight, const float priorErrorCovariance,
										   const float estimatedProcessNoiseCovariance) {
	return priorWeight * priorErrorCovariance + estimatedProcessNoiseCovariance;
}

float KalmanFilter::calculateGain(const float errorCovariance, const float estimatedMeasurementNoiseCovariance) {
	return errorCovariance / (errorCovariance + estimatedMeasurementNoiseCovariance);
}

float KalmanFilter::correctEstimate(const float estimate, float gain, const float measurement) {
	return estimate + gain * (measurement - estimate);
}

float KalmanFilter::correctErrorCovariance(const float gain, const float errorCovariance) {
	return (1.0f - gain) * errorCovariance;
}

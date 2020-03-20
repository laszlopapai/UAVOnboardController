#pragma once

#include <Data/StreamNode.h>

#include "Interact/Gyroscope.h"
#include "Interact/Magnetometer.h"
#include "Interact/Accelerometer.h"
#include "Interact/Barometer.h"
#include "Interact/PulseWidthModulator.h"

#include "Aircrafts/IAircraft.h"
#include "Structures/RemoteIn.h"

#include <HCSR04/HCSR04Rangefinder.h>
#include <Camera/CameraCapture.h>

namespace IoT {
	namespace UAV {

		class DeviceHandler
		{

		public:
			explicit DeviceHandler(float frequency);

			bool initialize();

			void setRotorSpeed(Core::DataInfo<PulseWidthSet>& dataInfo) const;

			void remoteHandler(Core::DataInfo<AircraftAppSignal>& dataInfo);

			Core::StreamPart& getGyroscopeOut() {
				return m_gyroscope;
			}

			Core::StreamPart& getAccelerometerOut() {
				return m_accelerometer;
			}

			Core::StreamPart& getMagnetometerOut() {
				return m_magnetometer;
			}

			Core::StreamNode<PulseWidthSet>& getRotorSpeedTransformIn() {
				return m_rotorSpeedTransform;
			}

			Core::StreamPart& getRemoteIn()
			{
				return m_remoteSignal;
			}

			AircraftAppSignal getSignal() const
			{
				return m_remoteSignal.current().getSignal();
			}

		private:
			Core::StreamPart* m_ticker;
			Core::ILogger *m_logger;

			Gyroscope m_gyroscope;
			Accelerometer m_accelerometer;
			Magnetometer m_magnetometer;
			Barometer m_barometer;

			PulseWidthModulator m_pulseWidthModulator;

			Device::HCSR04Rangefinder m_rangefinder;
			Device::CameraCapture m_cameraCapture;

			std::unique_ptr<IAircraft> m_aircraft;
			Core::StreamNode<PulseWidthSet> m_rotorSpeedTransform;
			Core::StreamNode<RemoteIn> m_remoteSignal;
		};
	}
}

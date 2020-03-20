#pragma once


#include "TimedVector3.h"
#include "KalmanFilterVector3.hpp"
#include "CalibratedFilter.h"

const int32_t AccelerometerCalibrationTime = (int32_t) 10e6;  // 10 sec in microsecs

namespace IoT {
	namespace Device {

		class AccelerometerFilter
			: public CalibratedFilter<Core::Vector3>
		{
		public:
			AccelerometerFilter();

			Core::Vector3 &getGravityVector() {
				return m_gravityVector;
			}
				
			bool isCalibrating() override;

			bool isCalibrated() override;

			bool calibrate(Core::Vector3 acceleration, uint32_t deltaTime = 0) override;

			void filter(Core::Vector3 & acceleration) override;

		private:
			Core::Vector3 m_gravityVector{ Core::Vector3::Down};
			float m_mean = 1;
			int32_t m_timeout = AccelerometerCalibrationTime;
			uint32_t m_meanCount = 0;
		};
	}
}

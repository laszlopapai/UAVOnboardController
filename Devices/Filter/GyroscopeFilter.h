#pragma once


#include <cfloat>
#include "TimedVector3.h"
#include "KalmanFilterVector3.hpp"
#include "CalibratedFilter.h"

namespace IoT {
	namespace Device {

		class GyroscopeFilter
			: public CalibratedFilter<Core::Vector3>
		{
		public:
			const int32_t GyroscopeCalibrationTime = (int32_t) 10e6; // 10 sec in microsecs

		public:
			GyroscopeFilter();

			bool isCalibrating() override;

			bool isCalibrated() override;

			bool calibrate(Core::Vector3 angularVelocity, uint32_t deltaTime) override;

			void filter(Core::Vector3 & angularVelocity) override;

		private:
			Core::Vector3 m_min{Core::Vector3(FLT_MAX, FLT_MAX, FLT_MAX)};
			Core::Vector3 m_max{Core::Vector3(FLT_MIN, FLT_MIN, FLT_MIN)};
			Core::Vector3 m_mean{Core::Vector3::Zero};
			int32_t m_timeout = GyroscopeCalibrationTime;
			uint32_t m_meanCount = 0;
		};
	}
}

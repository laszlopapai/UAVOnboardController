#pragma once

#include <cfloat>
#include "TimedVector3.h"
#include "KalmanFilterVector3.hpp"
#include "CalibratedFilter.h"

const int32_t MagnetometerCalibrationTime = (int32_t) 10e6; // 10 sec in microsecs

namespace IoT {
	namespace Device {

		class MagnetometerFilter
			: public CalibratedFilter<Core::Vector3>
		{
		public:
			MagnetometerFilter();

			bool isCalibrating() override;

			bool isCalibrated() override;

			bool calibrate(Core::Vector3 magnetism, uint32_t deltaTime = 0) override;

			void filter(Core::Vector3 & magnetism) override;

		private:
			Core::Vector3 m_min{ Core::Vector3(FLT_MAX, FLT_MAX, FLT_MAX) };
			Core::Vector3 m_max{ Core::Vector3(FLT_MIN, FLT_MIN, FLT_MIN) };
			int32_t m_timeout = MagnetometerCalibrationTime;
		};
	}
}

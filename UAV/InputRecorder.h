#pragma once

#include "Data/Recorder.h"
#include "Math/Vector3.h"

namespace IoT {
	namespace UAV {

		struct InputRecorder
		{

			InputRecorder(std::string testCase)
				: m_testCase(testCase)
				, m_loaded(true)
			{
				m_loaded &= m_gyroscope.loadData(m_testCase + ".gyroTestData");
				m_loaded &= m_accelerometer.loadData(m_testCase + ".accelTestData");
				m_loaded &= m_magnetometer.loadData(m_testCase + ".magnetTestData");
			}

			~InputRecorder()
			{
				m_gyroscope.saveData(m_testCase + ".gyroTestData");
				m_accelerometer.saveData(m_testCase + ".accelTestData");
				m_magnetometer.saveData(m_testCase + ".magnetTestData");
			}

			bool isLoaded() {
				return m_loaded;
			}


		public:
			Core::Recorder<Core::Vector3> m_gyroscope;
			Core::Recorder<Core::Vector3> m_accelerometer;
			Core::Recorder<Core::Vector3> m_magnetometer;

		private:
			std::string m_testCase;
			bool m_loaded;
		};

	}
}

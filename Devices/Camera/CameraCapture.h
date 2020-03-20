#pragma once

#include <opencv2/opencv.hpp>
#include <fstream>
#include <iostream>
#include <vector>

#include <Thread/AutoAwaitStopSignal.h>
#include <Thread/NotifyPipe.hpp>
#include <Thread/AsyncProcess.h>

namespace IoT {
	namespace Device {

		class CameraCapture
			: public Core::AsyncProcess
		{
		public:
			CameraCapture();

			bool initialize() override;

			Core::NotifyPipe<std::vector<uint8_t >*> & getCaptured() {
				return m_frameDataPipe;
			}

			void pause();

			void resume();

			bool isRunning() const;

			void onStop() override;

		private:
			void run() override;

		private:
			std::atomic_bool m_isRunning { false };
			Core::AwaitStopSignal m_awaitStopSignal;
			cv::Mat m_frame;
			cv::VideoCapture m_videoCapture;
			std::vector<int> m_convertParam;
			std::vector<uint8_t> m_buffer;
			Core::NotifyPipe<std::vector<uint8_t>*> m_frameDataPipe;
		};
	}
}

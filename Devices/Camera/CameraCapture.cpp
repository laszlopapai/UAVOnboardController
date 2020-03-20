#include <thread>
#include "CameraCapture.h"
#include <ILogger.h>

using namespace IoT::Device;
using namespace IoT::Core;

CameraCapture::CameraCapture()
	: m_convertParam(2)
{ }

bool CameraCapture::initialize() {
	// open the default camera using default API
	m_videoCapture.open(0);

	// OR advance usage: select any API backend
	int deviceID = 0;			 // 0 = open default camera
	int apiID = cv::CAP_ANY;	  // 0 = autodetect default API

	// open selected camera using selected API
	m_videoCapture.open(deviceID + apiID);

	// check if we succeeded
	if (!m_videoCapture.isOpened()) {
		ILogger::getDefault()->log(Error, "Camera", "Open CV unable to open camera device");
		return false;
	}

	m_convertParam[0] = cv::IMWRITE_JPEG_QUALITY;
	m_convertParam[1] = 80;//default(95) 0-100

	return true;
}

void CameraCapture::pause() {
	m_awaitStopSignal.reset();
	m_isRunning = false;
}

void CameraCapture::resume() {
	m_awaitStopSignal.setWake();
	m_isRunning = true;
}

void CameraCapture::run() {
	auto time = Time::getNow();
	uint32_t f = 0;
	while (m_awaitStopSignal.waitSignal() != AwaitStopSignal::Stop) {
		// wait for a new m_frame from camera and store it into 'm_frame'
		m_videoCapture.read(m_frame);

		// check if we succeeded
		if (m_frame.empty()) {
			ILogger::getDefault()->log(Error, "Camera", "Open CV grabbed a blank image from the camera");
			break;
		}

		// convert to jpg with data loss
		cv::imencode(".jpg", m_frame, m_buffer, m_convertParam);

		m_frameDataPipe.pushData(new std::vector<uint8_t >(m_buffer));

		f++;
		if ((Time::getNow() - time).getMicros() >= 1e6) {
			ILogger::getDefault()->log(Debug, "Camera", "FPS: " + std::to_string(f) + " (In the queue: " + std::to_string(m_frameDataPipe.getBufferedNum()) + ")");
			f = 0;
			time = Time::getNow();
		}
	}
}

void CameraCapture::onStop() {
	m_awaitStopSignal.setStop();
	m_isRunning = false;
}

bool CameraCapture::isRunning() const {
	return m_isRunning;
}

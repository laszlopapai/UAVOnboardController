#include "Config.h"
#include "AircraftApp.h"
#include <Thread/Utils.h>
#include <linux/reboot.h>
#include <sys/reboot.h>
#include <iomanip>

using namespace IoT::UAV;
using namespace IoT::Core;

AircraftApp::AircraftApp()
	: m_device(OutputFrequency)
	, m_remoteControl(IncomingInfoPort, OutgoingInfoPort, OutgoingCameraPort, PackagesPerSec)
	, m_inputRecorder("rotate")
	, m_consoleOut([this](DataInfo<bool>& dataInfo) { this->printConsole(dataInfo); })
{}

bool AircraftApp::initialize() {

	auto logger = ILogger::getDefault();
	logger->setLogLevelForConsole((LogLevel) (
			Debug   |
			Info	|
			Notice  |
			Warning |
			Error
	));
	logger->setLogLevelFile((LogLevel) (
			Debug   |
			Info	|
			Notice  |
			Warning |
			Error
	));

	InfoLog("Initialize...");

	bindThisThreadToCore(1);

	connectDependencies();

	return initializeModules();
}

void AircraftApp::connectDependencies() {

	StreamPart* gyroscopeSource = &m_device.getGyroscopeOut();
	StreamPart* accelerometerSource = &m_device.getAccelerometerOut();
	StreamPart* magnetometerSource = &m_device.getMagnetometerOut();

	if (PlaybackDevices) {
		gyroscopeSource = &m_inputRecorder.m_gyroscope;
		accelerometerSource = &m_inputRecorder.m_accelerometer;
		magnetometerSource = &m_inputRecorder.m_magnetometer;
	}

	if (!RecordDevices)
	{
		m_ticker | *gyroscopeSource | m_ahrs.getAngularVelocity() | m_pilote.getAngularVelocityIn();
		m_ticker | *accelerometerSource | m_ahrs.getAccelerationIn();
		m_ticker | *magnetometerSource | m_ahrs.getMagnetismIn();
		m_ticker | m_remoteControl.getTickerIn();
		m_ticker | m_consoleOut;

		m_ahrs.getAngularPositionOut() | m_remoteControl.getAngulerPositionSendIn();
		m_ahrs.getAngularPositionOut() | m_pilote.getAngularPositionIn();

		m_remoteControl.getControlReceiveOut() | m_pilote.getRemoteIn();
		m_remoteControl.getControlReceiveOut() | m_device.getRemoteIn();

		m_pilote.getAngularControlTrustOut() | m_device.getRotorSpeedTransformIn();
		m_device.getRotorSpeedTransformIn() | m_remoteControl.getRotorSpeedSendIn();
	}
	else
	{
		m_device.getGyroscopeOut() | m_inputRecorder.m_gyroscope;
		m_device.getAccelerometerOut() | m_inputRecorder.m_accelerometer;
		m_device.getMagnetometerOut() | m_inputRecorder.m_magnetometer;
	}
}

bool AircraftApp::initializeModules() {
	if (!PlaybackDevices && !m_device.initialize()) {
		ErrorLog("Device controller initialization failed, It's a mandatory module so the execution aborts.");
		return false;
	}
	else if (PlaybackDevices && !m_inputRecorder.isLoaded()) {
		ErrorLog("Test data record initialization failed, It's a mandatory module so the execution aborts.");
		return false;
	}
	if (!m_ahrs.initialize()) {
		ErrorLog("Analyzer initialization failed, It's a mandatory module so the execution aborts.");
		return false;
	}
	if (!m_pilote.initialize()) {
		ErrorLog("Pilot initialization failed, It's a mandatory module so the execution aborts.");
		return false;
	}
	if (!m_remoteControl.initialize()) {
		ErrorLog("Connection handler initialization failed, It's a mandatory module so the execution aborts.");
		return false;
	}
	
	return true;
}

void AircraftApp::printConsole(DataInfo<bool>& dataInfo)
{
	dataInfo.m_propagate = true;
	if (m_consoleOut.ellapsed().getSecs<float>() < 0.1f) {
		dataInfo.m_propagate = false;
		return;
	}

	const auto num2str = [](auto n, uint8_t d)
	{
		std::stringstream stream;
		stream << std::fixed << std::setprecision(d) << std::abs(n);
		return stream.str();
	};

	const auto middleSlide = [&](float p, uint8_t l, uint8_t d)
	{
		const int n = static_cast<int>(p / M_PI * l);
		const int leftTagNum = std::abs(std::min(n, 0));
		const int rightTagNum = std::max(n, 0);

		std::string out = " [";

		out.append(std::max(l - leftTagNum, 0), ' ');
		out.append(leftTagNum, '#');

		out += num2str(p, d);

		out.append(rightTagNum, '#');
		out.append(std::max(l - rightTagNum, 0), ' ');

		return out + "] ";
	};

	const auto leftSlide = [&](float p, uint8_t l, uint8_t d)
	{
		const int n = std::max(static_cast<int>(p * l), 0);

		std::string out = " [";

		if (p >= 0)
			out += num2str(p, d);
		else
			out.append(d + 2, ' ');

		out.append(std::max(n, 0), '#');
		out.append(std::max(l - n, 0), ' ');
		return out + "] ";
	};

	auto acc = m_inputRecorder.m_accelerometer.current();
	auto gyr = m_inputRecorder.m_gyroscope.current();
	auto angPos = m_ahrs.getAngularPositionOut().current();
	auto angCtrTru = m_pilote.getAngularControlTrustOut().current().toString();
	auto pwm = m_device.getRotorSpeedTransformIn().current();

	std::cout << middleSlide(angPos.x, 15, 3);
	std::cout << middleSlide(angPos.y, 15, 3);
	std::cout << middleSlide(angPos.z, 10, 3);

	std::cout << leftSlide(pwm.getESCValue(3), 10, 3);
	std::cout << leftSlide(pwm.getESCValue(2), 10, 3);
	std::cout << leftSlide(pwm.getESCValue(1), 10, 3);
	std::cout << leftSlide(pwm.getESCValue(0), 10, 3);

	std::cout << "@ " << std::fixed << std::setprecision(3) << m_inputRecorder.m_accelerometer.state() * 100;
	//std::cout << "AngPos: " << angPos.toString(); // << " angCtrl: " << angCtrTru	

	std::cout << std::endl;;
}

int AircraftApp::run() {

	m_remoteControl.start();
	InfoLog("Controller starting");

	const auto stopSignal = AircraftAppSignal::Stop | AircraftAppSignal::Reboot | AircraftAppSignal::Shutdown;
	while (!m_interrupt && (m_device.getSignal() & stopSignal) == 0) {
		m_ticker.trigger();
	}

	m_remoteControl.stop();

	if (m_device.getSignal() & AircraftAppSignal::Reboot) {
		InfoLog("System reboot");
		reboot(LINUX_REBOOT_CMD_RESTART);
	}
	else if (m_device.getSignal() & AircraftAppSignal::Shutdown) {
		InfoLog("System shutdown");
		reboot(LINUX_REBOOT_CMD_POWER_OFF);
	}

	return 0;
}

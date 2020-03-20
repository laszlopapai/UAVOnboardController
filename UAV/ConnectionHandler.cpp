#include "ConnectionHandler.h"
#include "Logger.h"
#include "Structures/PulseWidthSet.h"


using namespace IoT::UAV;

ConnectionHandler::ConnectionHandler(const uint16_t controlInPort, const uint16_t controlOutPort, const uint16_t cameraOutPort, const uint16_t packagesPerSec)
	: m_logger(Logger::getDefault())

	, m_timeBetweenPackage(1e6f / packagesPerSec)
	
	, m_controlClient(controlInPort)
	, m_controlServer(controlOutPort)
	, m_cameraServer(cameraOutPort)

	, m_receiveThrotler([this](auto& dataInfo) { this->receiveThrotler(dataInfo); })
	, m_sendThrotler([this](auto& dataInfo) { this->sendThrotler(dataInfo); })
{}

bool ConnectionHandler::initialize()
{
	m_ticker | m_receiveThrotler | m_controlClient | m_controlReceive;
	
	m_ticker | m_sendThrotler | m_controlServer;
	m_angularPositionSend | m_sendThrotler;
	m_rotorSpeedSend | m_sendThrotler;

	//m_cameraSend | m_cameraServer;
		

	if (!m_controlServer.initialize()) {
		m_logger->log(Core::Error, "ConnectionHandler",
			"Remote sender initialization failed, It's a mandatory module so the execution aborts.");
		return false;
	}	

	if (!m_controlClient.initialize()) {
		m_logger->log(Core::Warning, "ConnectionHandler",
					  "Camera initialization failed, It's not a mandatory module so the execution continues.");
	}

	if (!m_cameraServer.initialize()) {
		m_logger->log(Core::Warning, "ConnectionHandler",
					  "Camera sender initialization failed, It's not a mandatory module so the execution continues.");
	}

	m_controlClient.setConnectionInfoCallback([&](Core::InternetSocketAddress& senderAddress) {
		m_controlServer.setEndpoint(senderAddress);
		m_cameraServer.setEndpoint(senderAddress);
	});

	return true;
}

void ConnectionHandler::start()
{
	m_cameraServer.start();
	m_controlServer.start();
	m_controlClient.start();
}

void ConnectionHandler::stop()
{
	m_controlClient.stop();
	m_controlServer.stop();
	m_cameraServer.stop();
}

void ConnectionHandler::receiveThrotler(Core::DataInfo<bool>& dataInfo) const
{
	// TODO: Is it realy necessary to throtle?
	dataInfo.m_propagate = dataInfo.m_this->ellapsed() >= m_timeBetweenPackage;
}

void ConnectionHandler::sendThrotler(Core::DataInfo<RemoteOut>& dataInfo) const
{
	dataInfo.m_propagate = dataInfo.m_this->ellapsed() >= m_timeBetweenPackage;
	if (!dataInfo.m_propagate)
		return;
	
	const auto& angPos = m_angularPositionSend.current();
	const auto& pulseWidthSet = m_rotorSpeedSend.current();
	
	dataInfo.m_data.m_angularPosition = angPos;
	for (int i = 0; i < 4; i++)
		dataInfo.m_data.m_rotorSpeed[i] = pulseWidthSet.getESCValue(i);
}

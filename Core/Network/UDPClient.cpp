#include "UDPClient.h"
#include<unistd.h>
#include<cstring>
#include <vector>

const int BufferLength = 65537;

using namespace IoT::Core;

UDPClient::UDPClient(std::function<void(std::vector<uint8_t> &)> process, uint16_t port)
	: m_processPackege({ std::move(process) })
	, m_logger(ILogger::getDefault())
	, m_stop(false)
	, m_sizeOfAddress(sizeof(InternetSocketAddress))
	, m_localSocket(-1)
	, m_port(port)
{ }

bool UDPClient::initialize() {

	if ((m_localSocket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) == -1)
	{
		m_logger->log(Error, "NetworkInput", "Cannot create udp socket");
		return false;
	}

	memset((char *)&m_localAddress, 0, sizeof(m_localAddress));
	m_localAddress.sin_family = AF_INET;
	m_localAddress.sin_port = htons(m_port);
	m_localAddress.sin_addr.s_addr = htonl(INADDR_ANY);

	if(bind(m_localSocket, (SocketAddressData*)&m_localAddress, m_sizeOfAddress) == -1) {
		m_logger->log(Error, "NetworkInput", "Cannot bind udp socket to the port: " + std::to_string(m_port));
		return false;
	}

	struct timeval tv;
	tv.tv_sec = 0;
	tv.tv_usec = 100000;
	if (setsockopt(m_localSocket, SOL_SOCKET, SO_RCVTIMEO, &tv, sizeof(tv)) < 0) {
		m_logger->log(Error, "NetworkInput", "Failed to setup socket receive timeout.");
		return false;
	}

	return true;
}

void UDPClient::run() {
	ssize_t length = 0;
	std::vector<uint8_t> buffer(BufferLength);

	do {
		length = recvfrom(m_localSocket, buffer.data(), BufferLength, 0, (SocketAddressData*)&m_remoteAddress, &m_sizeOfAddress);

		if (length > 0 && length < BufferLength) {
			buffer.resize(length);
			m_connectionInfoCallback(m_remoteAddress);
			m_processPackege(buffer);
		}

	} while(!m_stop);
}

void UDPClient::onStop() {
	m_stop = true;

	if (m_localSocket > 0) {
		close(m_localSocket);
	}
}

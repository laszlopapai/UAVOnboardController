#include "UDPServer.h"
#include <cstring>
#include <unistd.h>
#include <string>
#include <cassert>

using namespace IoT::Core;

UDPServer::UDPServer(std::function<bool(std::vector<uint8_t> &)> process, uint16_t port)
	: m_processPackege({ std::move(process) })
	, m_port(port)
	, m_sizeOfAddress(sizeof(InternetSocketAddress))
{ }

bool UDPServer::initialize() {

	//create a UDP socket
	if ((m_localSocket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) == -1)
	{
		m_logger->log(Error, "NetworkOutput", "Cannot create udp socket");
		return false;
	}

	// zero out the structure
	memset((char *)&m_remoteAddress, 0, sizeof(m_remoteAddress));

	m_remoteAddress.sin_family = AF_INET;
	m_remoteAddress.sin_port = htons(m_port);

	int broadcastEnable=1;
	if (setsockopt(m_localSocket, SOL_SOCKET, SO_BROADCAST, &broadcastEnable, sizeof(broadcastEnable))) {
		close(m_localSocket);
		m_localSocket = -1;
		m_logger->log(Error, "NetworkOutput", "Failed to enable broadcast.");
		return false;
	}

	if (inet_aton("255.255.255.255" , &m_remoteAddress.sin_addr) == 0) {
		close(m_localSocket);
		m_localSocket = -1;
		m_logger->log(Error, "NetworkOutput", "Failed to set ip remote address");
		return false;
	}

	return true;
}

void UDPServer::run() {
	assert(m_localSocket != -1);
	std::vector<uint8_t> data;

	while (m_processPackege(data)) {
		std::unique_lock<std::mutex> lock(m_endpointProtect);
		sendto(m_localSocket, data.data(), data.size(), 0, (SocketAddressData*)&m_remoteAddress, m_sizeOfAddress);
	}
}

UDPServer::~UDPServer()
{
	if (m_localSocket > -1)
		close(m_localSocket);
	m_localSocket = -1;
}

void UDPServer::setEndpoint(InternetSocketAddress &sa) {
	std::unique_lock<std::mutex> lock(m_endpointProtect);

	if (sa.sin_addr.s_addr == m_remoteAddress.sin_addr.s_addr)
		return;

	m_remoteAddress.sin_addr.s_addr = sa.sin_addr.s_addr;

	m_logger->log(Info, "NetworkOutput", "The udp remote set to: " +
			std::string(inet_ntoa(m_remoteAddress.sin_addr)) + std::string(":") +
			std::to_string(ntohs(m_remoteAddress.sin_port))
	);
}

#pragma once

#include <arpa/inet.h>
#include <mutex>
#include <vector>
#include <ILogger.h>
#include <Thread/AsyncProcess.h>
#include <functional>

namespace IoT {
	namespace Core {

		typedef struct sockaddr_in InternetSocketAddress;
		typedef struct sockaddr SocketAddressData;

		class UDPServer
			: public AsyncProcess
		{

		public:
			UDPServer(std::function<bool(std::vector<uint8_t> &)> process, uint16_t port);

			bool initialize() override;

			void run() override;

			void setEndpoint(InternetSocketAddress &sa);

			virtual ~UDPServer();

		private:
			ILogger *m_logger{ILogger::getDefault()};
			std::mutex m_endpointProtect;
			std::function<bool(std::vector<uint8_t> &)> m_processPackege;

			socklen_t m_sizeOfAddress;
			InternetSocketAddress m_remoteAddress;
			int m_localSocket;
			uint16_t m_port;
		};
	}
}

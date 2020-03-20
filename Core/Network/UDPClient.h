#pragma once

#include <arpa/inet.h>
#include <ILogger.h>
#include <Thread/AsyncProcess.h>
#include <vector>
#include <atomic>
#include <functional>

namespace IoT {
	namespace Core {

		typedef struct sockaddr_in InternetSocketAddress;
		typedef struct sockaddr SocketAddressData;

		class UDPClient
			: public AsyncProcess
		{
		public:
			UDPClient(std::function<void(std::vector<uint8_t> &)> process, uint16_t port);

			bool initialize() override;

			void onStop() override;

			void setConnectionInfoCallback(std::function<void(InternetSocketAddress &)> callback) {
				m_connectionInfoCallback = std::move(callback);
			}

		private:
			void run() override;

			ILogger *m_logger;
			std::atomic_bool m_stop;

			socklen_t m_sizeOfAddress;
			InternetSocketAddress m_localAddress;
			InternetSocketAddress m_remoteAddress;
			int m_localSocket;
			uint16_t m_port;

			std::function<void(std::vector<uint8_t> &)> m_processPackege;
			std::function<void(InternetSocketAddress &)> m_connectionInfoCallback;
		};
	}
}

#pragma once

#include "UDPServer.h"
#include "Data/StreamNode.h"
#include "Thread/AutoAwaitStopSignal.h"

namespace IoT {
	namespace Core {

		template<typename T>
		class PackageServer
			: public StreamNode<bool> {

		public:
			PackageServer(uint16_t port = 2001)
				: StreamNode<bool>([this](auto& dataInfo) { this->sendPackege(dataInfo); })
				, m_udpServer([this](auto& v) { return this->processPackage(v); }, port)
			{}

			bool initialize() {
				return m_udpServer.initialize();
			}

			void setEndpoint(InternetSocketAddress &sa) {
				m_udpServer.setEndpoint(sa);
			}

			void stop() {
				m_autoAwaitStopSignal.setStop();
				m_udpServer.stop();
			}

			void start() {
				m_udpServer.start();
			}

		private:
			void sendPackege(DataInfo<bool>& dataInfo)
			{
				std::unique_lock<std::mutex> lock(m_dataProtect);
				m_data = dataInfo.getSource<T>().current();
				dataInfo.m_data = true;
				dataInfo.m_propagate = true;
				m_autoAwaitStopSignal.setWake();
			}

			bool processPackage(std::vector<uint8_t> &data) {
				if (m_autoAwaitStopSignal.waitSignal() == AutoAwaitStopSignal::Stop) {
					return false;
				}

				std::unique_lock<std::mutex> lock(m_dataProtect);
				m_data.serialize(data);

				return true;
			}

		private:
			std::mutex m_dataProtect;
			T m_data;
			AutoAwaitStopSignal m_autoAwaitStopSignal{};
			UDPServer m_udpServer;
		};
	}
}

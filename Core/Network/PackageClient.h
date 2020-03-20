#pragma once

#include <Thread/AtomicMethod.hpp>
#include "UDPClient.h"
#include "Data/StreamNode.h"

namespace IoT {
	namespace Core {

		template<typename T>
		class PackageClient
			: public StreamNode<T>
		{

		public:
			PackageClient(uint16_t port = 2000)
				: StreamNode<T>([this](auto& dataInfo) { this->getPackage(dataInfo); })
				, m_udpClient([this](auto& v) { this->processPackage(v); }, port)
			{}

			bool initialize() {
				return m_udpClient.initialize();
			}

			void setConnectionInfoCallback(std::function<void(InternetSocketAddress &)> callback) {
				m_udpClient.setConnectionInfoCallback(std::move(callback));
			}

			void start() {
				m_udpClient.start();
			}

			void stop() {
				m_udpClient.stop();
			}

		private:
			void getPackage(DataInfo<T>& dataInfo) {
				std::unique_lock<std::mutex> lock(m_dataProtect);
				dataInfo.m_data = m_data;
				dataInfo.m_propagate = true;
			}

			void processPackage(const std::vector<uint8_t> &data) {
				std::unique_lock<std::mutex> lock(m_dataProtect);
				m_data.deserialize(data);
			}

		private:
			std::mutex m_dataProtect;
			T m_data;
			UDPClient m_udpClient;
		};
	}
}

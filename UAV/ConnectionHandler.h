#pragma once

#include "Data/StreamBinder.h"
#include "Network/PackageServer.h"
#include "Structures/RemoteCameraOut.h"
#include "Structures/RemoteOut.h"
#include "Network/PackageClient.h"
#include "Structures/RemoteIn.h"
#include "Structures/PulseWidthSet.h"

namespace IoT {
	namespace UAV {
		class ConnectionHandler
		{
			enum StreamTags : int
			{
				Ticker,
				ControlReceive,
				AngularPositionSend,
				RotorSpeedSend,
				CameraSend
			};

		public:
			ConnectionHandler(uint16_t controlInPort, uint16_t controlOutPort, uint16_t cameraOutPort, uint16_t packagesPerSec);

			~ConnectionHandler() = default;

			bool initialize();

			void start();

			void stop();

			void receiveThrotler(Core::DataInfo<bool>& dataInfo) const;

			void sendThrotler(Core::DataInfo<RemoteOut>& dataInfo) const;

			Core::StreamPart& getTickerIn() {
				return m_ticker;
			}

			Core::StreamPart& getAngulerPositionSendIn() {
				return m_angularPositionSend;
			}

			Core::StreamPart& getRotorSpeedSendIn() {
				return m_rotorSpeedSend;
			}

			Core::StreamPart& getControlReceiveOut() {
				return m_controlReceive;
			}

			Core::StreamPart& getCameraSendIn() {
				return m_cameraSend;
			}


		private:
			Core::ILogger *m_logger;

			Core::Time m_timeBetweenPackage;

			Core::StreamBinder m_ticker;
			Core::StreamNode<Core::Vector3> m_angularPositionSend;
			Core::StreamNode<PulseWidthSet> m_rotorSpeedSend;

			Core::StreamBinder m_cameraSend;
			Core::StreamBinder m_controlReceive;

			Core::PackageClient<RemoteIn> m_controlClient;
			Core::PackageServer<RemoteOut> m_controlServer;
			Core::PackageServer<RemoteCameraOut> m_cameraServer;

			Core::StreamNode<bool> m_receiveThrotler;
			Core::StreamNode<RemoteOut> m_sendThrotler;
		};

	}
}

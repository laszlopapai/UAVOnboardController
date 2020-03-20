#pragma once


#include <Thread/AutoAwaitStopSignal.h>

#include <Network/PackageClient.h>
#include <Network/PackageServer.h>

#include "DeviceHandler.h"
#include "Structures/RemoteCameraOut.h"
#include "Logger.h"
#include "SensorReferenceSystemB.h"
#include "InputRecorder.h"
#include "Pilot.h"
#include "ConnectionHandler.h"

namespace IoT {
	namespace UAV {
		class AircraftApp
		{

		public:
			AircraftApp();

			bool initialize();

			int run();

			void interrupt() { m_interrupt = true; }

		private:
			void connectDependencies();

			bool initializeModules();

			void printConsole(Core::DataInfo<bool>& dataInfo);

		private:
			DeviceHandler m_device;
			ConnectionHandler m_remoteControl;
			SensorReferenceSystemB m_ahrs;
			Pilot m_pilote;
			InputRecorder m_inputRecorder;

			Core::StreamPart m_ticker;
			Core::StreamNode<bool> m_consoleOut;

			bool m_interrupt{ false };
		};
	}
}

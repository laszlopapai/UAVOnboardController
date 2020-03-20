#pragma once

#include <vector>
#include <fstream>
#include <unordered_map>
#include "../Core/ILogger.h"

namespace IoT {
	namespace UAV {

		class Logger
			: public Core::ILogger
		{

			static const std::string logLevelNames[5];

		protected:
			Logger();

		public:
			static void setupDefault() {
				ILogger::s_instance = new Logger;
			}

			void log(Core::LogLevel logLevel, std::string category, std::string message) override;

			void setLogLevelForConsole(Core::LogLevel logLevel) override;

			void setLogLevelForBuffer(Core::LogLevel logLevel) override;

			void setLogLevelFile(Core::LogLevel logLevel) override;

			void getLogs(std::string **logs, uint32_t *numLogs) override;

			void clearLogs() override;

			virtual ~Logger();

		private:
			std::vector<std::string> m_logs;
			std::ofstream m_fileOut;

			uint8_t m_consoleLogLevel = 0;
			uint8_t m_bufferLogLevel = 0;
			uint8_t m_fileLogLevel = 0;

			std::unordered_map<std::string, bool> m_consoleCategory;
			std::unordered_map<std::string, bool> m_bufferCategory;
			std::unordered_map<std::string, bool> m_fileCategory;
		};
	}
}

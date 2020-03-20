#pragma once

#define STRINGIFY(x) #x
#define TOSTRING(x) STRINGIFY(x)
#define DebugLog(msg) IoT::Core::ILogger::getDefault()->log(Debug, __FILE__ ":" TOSTRING(__LINE__), msg)
#define InfoLog(msg) IoT::Core::ILogger::getDefault()->log(Info, __FILE__ ":" TOSTRING(__LINE__), msg)
#define WarningLog(msg) IoT::Core::ILogger::getDefault()->log(Warning, __FILE__ ":" TOSTRING(__LINE__), msg)
#define ErrorLog(msg) IoT::Core::ILogger::getDefault()->log(Error, __FILE__ ":" TOSTRING(__LINE__), msg)

#include <string>
#include <cstdint>

namespace IoT {
	namespace Core {

		enum LogLevel : uint8_t {
			Debug   = 1,
			Info	= 2,
			Notice  = 4,
			Warning = 8,
			Error   = 16,
		};

		class ILogger {
		protected:
			static ILogger* s_instance;

		public:
			static ILogger* getDefault() {
				return s_instance;
			}
			virtual void log			 (LogLevel logLevel, std::string category, std::string message) = 0;

			virtual void setLogLevelForConsole(LogLevel logLevel) = 0;
			virtual void setLogLevelForBuffer (LogLevel logLevel) = 0;
			virtual void setLogLevelFile	  (LogLevel logLevel) = 0;

			virtual void getLogs		 (std::string **logs, uint32_t *numLogs) = 0;
			virtual void clearLogs	   () = 0;
		};

	}
}

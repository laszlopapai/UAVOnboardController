#include <cstdarg>
#include <iostream>
#include "Logger.h"
#include <cmath>
#include <strings.h>
#include <ctime>

using namespace IoT::Core;

ILogger* ILogger::s_instance = nullptr;

using namespace IoT::UAV;

const std::string Logger::logLevelNames[5] = {
		"Debug: ",
		"Info: ",
		"Notice: ",
		"Warning: ",
		"Error: "
};

Logger::Logger() {
	time_t rawtime;
	struct tm * timeinfo;
	char fileName [100];

	time (&rawtime);
	timeinfo = localtime (&rawtime);

	strftime (fileName, sizeof(fileName), "%Y%m%d-%H%M.log",timeinfo);

	m_fileOut.open(fileName);
}

void Logger::log(LogLevel logLevel, std::string category, std::string message) {
	auto logLevelNum = (uint8_t)ffs(logLevel) - 1;
	if (logLevelNum < 0) {
		return;
	}

	std::string log = logLevelNames[logLevelNum] + message + " (" + category + ")";

	if ((m_bufferLogLevel & logLevel) > 0) {
		m_logs.emplace_back(log);
	}
	if ((m_consoleLogLevel & logLevel) > 0) {
		std::cout << log << std::endl;
	}
	if ((m_fileLogLevel & logLevel) > 0) {
		m_fileOut << log << std::endl;
	}
}

void Logger::getLogs(std::string **logs, uint32_t *numLogs) {
	*logs	= m_logs.data();
	*numLogs = (uint32_t)m_logs.size();
}

void Logger::clearLogs() {
	m_logs.clear();
}

Logger::~Logger() {
	if (m_fileOut.is_open()) {
		m_fileOut.close();
	}
}

void Logger::setLogLevelForBuffer(LogLevel logLevel) {
	m_bufferLogLevel = logLevel;
}

void Logger::setLogLevelForConsole(LogLevel logLevel) {
	m_consoleLogLevel = logLevel;
}

void Logger::setLogLevelFile(LogLevel logLevel) {
	m_fileLogLevel = logLevel;
}
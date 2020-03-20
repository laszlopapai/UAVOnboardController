#pragma once

#include "UAV/AircraftApp.h"
#include <csignal>

class Application {

public:
	int exec();

	~Application();

private:
	static bool initialize();

	static void actionHandler(int sig, siginfo_t *info, void *args);

	static void printCallStack(IoT::Core::ILogger *logger);

	static std::string exec(const char* cmd);

private:
	static IoT::UAV::AircraftApp * s_app;
};

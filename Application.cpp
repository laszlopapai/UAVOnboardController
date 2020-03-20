#include "Application.h"
#include <execinfo.h>

using namespace IoT::UAV;
using namespace IoT::Core;

int main() {
	Application a;
	return a.exec();
}

AircraftApp* Application::s_app = nullptr;

int Application::exec()
{
	if (s_app) {
		Logger::getDefault()->log(Error, "MainApplication", "The application has already started!");
		return 1;
	}

	if (!initialize())
		return 1;

	return s_app->run();
}

Application::~Application()
{
	delete s_app;
	s_app = nullptr;
}

bool Application::initialize()
{
	Logger::setupDefault();
	s_app = new AircraftApp;

	struct sigaction sa;
	memset(&sa, 0, sizeof(struct sigaction));
	sigemptyset(&sa.sa_mask);
	sa.sa_flags = SA_RESTART | SA_SIGINFO;
	sa.sa_sigaction = actionHandler;

	sigaction(SIGSEGV, &sa, NULL);
	sigaction(SIGINT, &sa, NULL);

	return s_app->initialize();
}

void Application::actionHandler(int sig, siginfo_t* info, void* args)
{
	ILogger *logger = Logger::getDefault();

	if (sig == SIGINT) {
		logger->log(Info, "MainApplication", "Interrupt signal received");
		s_app->interrupt();
	}
	else if (sig == SIGSEGV) {
		char msg[100];
		sprintf(msg, "Segmentation fault, faulty address is %p", info->si_addr);
		logger->log(Error, "MainApplication", msg);
		printCallStack(logger);
		exit(0);
	}
	else {
		logger->log(Info, "MainApplication", "Got signal " + std::to_string(sig));
	}
}

void Application::printCallStack(ILogger* logger)
{
	void *trace[16];
	char **messages = (char **)NULL;
	int trace_size = 0;

	trace_size = backtrace(trace, 16);
	messages = backtrace_symbols(trace, trace_size);

	logger->log(Error, "MainApplication", "Call stack:");
	for (int i = 3; i < trace_size; ++i)
	{
		logger->log(Error, "MainApplication", messages[i]);

		/* find first occurence of '(' or ' ' in message[i] and assume
		* everything before that is the file name. (Don't go beyond 0 though
		* (string terminator)*/
		size_t p = 0;
		while (messages[i][p] != '(' && messages[i][p] != ' ' && messages[i][p] != 0)
			++p;

		char syscom[256];
		sprintf(syscom, "addr2line %p -e %.*s", trace[i], p, messages[i]);
		//last parameter is the filename of the symbol
		logger->log(Error, "MainApplication", exec(syscom));
	}
}

std::string Application::exec(const char* cmd)
{
	char buffer[128];
	std::string result = "";
	FILE* pipe = popen(cmd, "r");
	if (!pipe) throw std::runtime_error("popen() failed!");
	try {
		while (!feof(pipe)) {
			if (fgets(buffer, 128, pipe) != NULL)
				result += buffer;
		}
	}
	catch (...) {
		pclose(pipe);
		throw;
	}
	pclose(pipe);

	// Trim the line ending
	result.erase(std::find_if(result.rbegin(), result.rend(), [](int ch) {
		return !std::isspace(ch);
	}).base(), result.end());

	return result;
}

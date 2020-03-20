#include "AsyncProcess.h"
#include <cassert>

using namespace IoT::Core;

void AsyncProcess::addSubProcess(AsyncProcess* process)
{
	m_subProcesses.push_back(process);
}

std::thread *AsyncProcess::start() {
	if (m_thread != nullptr) {
		return m_thread;
	}

	m_thread = new std::thread(&AsyncProcess::run, this);
	return m_thread;
}

void AsyncProcess::stop() {
	assert(m_thread != nullptr);

	onStop();

	m_thread->join();
	delete m_thread;
	m_thread = nullptr;
}

AsyncProcess::~AsyncProcess() {
	delete m_thread;
}

void AsyncProcess::run()
{
	for (auto process : m_subProcesses) {
		process->run();
	}
}

void AsyncProcess::stopSubproceses()
{
	for (auto process : m_subProcesses) {
		process->onStop();
	}
}

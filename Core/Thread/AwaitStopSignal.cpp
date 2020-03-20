#include "AwaitStopSignal.h"
#include <iostream>
#include <chrono>

using namespace IoT::Core;

AwaitStopSignal::AwaitStopSignal(Signal signal)
	: m_state(signal)
{ }


uint8_t AwaitStopSignal::waitSignal() {
	std::unique_lock<std::mutex> lock(m_mutex);
	while (m_state == Await) {
		m_conditionalVariable.wait(lock);
	}

	return m_state;
}


uint8_t AwaitStopSignal::waitSignal(uint16_t time) {
	std::unique_lock<std::mutex> lock(m_mutex);
	while (m_state == Await) {
		m_conditionalVariable.wait_for(lock, std::chrono::milliseconds(time));
	}
}

uint8_t AwaitStopSignal::getSignal() {
	std::unique_lock<std::mutex> lock(m_mutex);
	return m_state;
}

void AwaitStopSignal::setCustom(uint8_t signal) {
	std::unique_lock<std::mutex> lock(m_mutex);

	if (m_state == Stop)
		return;

	m_state = signal;
	m_conditionalVariable.notify_all();
}

void AwaitStopSignal::setWake() {
	std::unique_lock<std::mutex> lock(m_mutex);

	if (m_state == Stop)
		return;

	m_state = Awake;
	m_conditionalVariable.notify_all();
}

void AwaitStopSignal::setStop() {
	std::unique_lock<std::mutex> lock(m_mutex);
	m_state = Stop;
	m_conditionalVariable.notify_all();
}

void AwaitStopSignal::reset() {
	std::unique_lock<std::mutex> lock(m_mutex);
	m_state = Await;
}

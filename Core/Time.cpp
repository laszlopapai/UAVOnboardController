#include "Time.h"

using namespace IoT::Core;

const Time Time::Zero(0);
uint32_t Time::m_epochMicro = 0;

Time::Time(const uint32_t micros)
	: m_micros(micros)
{
	if (m_epochMicro == 0) {
		struct timespec ts;

		clock_gettime(CLOCK_MONOTONIC_RAW, &ts);
		m_epochMicro = (uint32_t)ts.tv_sec * (uint32_t)1000000 + (uint32_t)(ts.tv_nsec / 1000);
	}
}

Time Time::getNow()
{
	struct  timespec ts;

	clock_gettime(CLOCK_MONOTONIC_RAW, &ts);
	const uint32_t now = (uint32_t)ts.tv_sec * (uint32_t)1000000 + (uint32_t)(ts.tv_nsec / 1000);

	return Time(now - m_epochMicro);
}

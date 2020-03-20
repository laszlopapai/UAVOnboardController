#pragma once

#include "AwaitStopSignal.h"
#include <chrono>

namespace IoT {
	namespace Core {

		class AutoAwaitStopSignal
			: public AwaitStopSignal
		{

		public:
			virtual ~AutoAwaitStopSignal() = default;

			uint8_t waitSignal() override {
				std::unique_lock<std::mutex> lock(m_mutex);
				while (m_state == Await) {
					m_conditionalVariable.wait(lock);
				}

				uint8_t state = m_state;
				m_state = Await;
				return state;
			}

			uint8_t waitSignal(uint16_t time) override {
				std::unique_lock<std::mutex> lock(m_mutex);
				while (m_state == Await) {
					if (!m_conditionalVariable.wait_for(lock, std::chrono::milliseconds(time),
														[] { return false; })) {
						if (m_state == Await) {
							m_state = Timeout;
						}
					}
				}

				uint8_t state = m_state;
				m_state = Await;
				return state;
			}
		};
	}
}

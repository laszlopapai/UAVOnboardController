#pragma once

#include <mutex>
#include <condition_variable>
#include <atomic>

namespace IoT {
	namespace Core {

		class AwaitStopSignal
		{
		public:
			enum Signal : uint8_t {
				Await = 255,
				Awake = 254,
				Timeout = 253,
				Stop = 252
			};

		public:
			explicit AwaitStopSignal(Signal signal = Await);

			virtual ~AwaitStopSignal() = default;

			uint8_t getSignal();

			virtual uint8_t waitSignal();

			virtual uint8_t waitSignal(uint16_t time);

			void setCustom(uint8_t);

			void setWake();

			void setStop();

			void reset();


		protected:
			std::mutex m_mutex;
			std::atomic<uint8_t> m_state;
			std::condition_variable m_conditionalVariable;
		};
	}
}

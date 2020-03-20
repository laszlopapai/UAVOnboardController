#pragma once

#include <cstdint>
#include <chrono>
#include <stdint.h>

namespace IoT {
	namespace Core {

		class Time
		{
		public:
			static const Time Zero;

		private:
			static uint32_t m_epochMicro;

		public:
			explicit Time(const uint32_t micros = 0);

			static Time getNow();

			uint64_t getMicros() const {
				return m_micros;
			}

			uint64_t getMillis() const {
				return static_cast<uint32_t>(m_micros * 1e-3f);
			}

			uint64_t getSecs() const {
				return static_cast<uint32_t>(m_micros * 1e-6f);
			}

			template<typename T>
			T getSecs() const {
				return static_cast<T>(m_micros * 1e-6f);
			}

			Time operator-(const Time & time) const {
				return Time(m_micros - time.m_micros);
			}


			bool operator==(const Time& rhs) const
			{
				return m_micros == rhs.m_micros;
			}

			bool operator!=(const Time& rhs) const
			{
				return !(*this == rhs);
			}

			bool operator<(const Time& rhs) const
			{
				return m_micros < rhs.m_micros;
			}

			bool operator<=(const Time& rhs) const
			{
				return !(rhs < *this);
			}

			bool operator>(const Time& rhs) const
			{
				return rhs < *this;
			}

			bool operator>=(const Time& rhs) const
			{
				return !(*this < rhs);
			}

		private:
			uint32_t m_micros{ 0 };
		};

		template<typename T>
		struct Timed {
			T data;
			Time time;
		};
	}
}

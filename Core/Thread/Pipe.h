#pragma once

#include <queue>
#include <Time.h>

namespace IoT {
	namespace Core {

		template<typename T>
		class Pipe
		{

		public:
			virtual ~Pipe() = default;

			virtual size_t getBufferedNum() {
				return m_data.size();
			}

			virtual void pushData(T data, const Time time) {
				Timed<T> a;

				a.data = data;
				a.time = time;

				m_data.push(a);
			}

			virtual bool popData(T *data) {
				Time time;
				return popData(data, &time);
			}

			virtual bool popData(T *data, Time *time) {

				if (m_data.empty())
					return false;

				Timed<T>& timedData = m_data.front();
				m_data.pop();

				*data = timedData.data;
				*time = timedData.time;

				return true;
			}

		protected:
			std::queue<Timed<T>> m_data;
		};
	}
}

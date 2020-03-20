#pragma once

#include "NotifyPipe.hpp"
#include "IObservable.h"
#include <mutex>

namespace IoT {
	namespace Core {

		template<typename T>
		class AtomicMethod
			: public IObserver
			, public IObservable
		{

		public:
			void latestFromPipe(NotifyPipe<T*> *pipe) {
				m_pipe = pipe;
				pipe->setObserver(this);
			}

			void setObserver(IObserver *observer) override {
				m_observer = observer;
			}

			void triggerChange() const {
				m_observer->observableChanged();
			}

			void observableChanged() override {
				T *data;
				if (m_pipe->popData(&data)) {
					set(*data);
					delete data;
					m_observer->observableChanged();
				}
			}

			void set(T &data) {
				std::unique_lock<std::mutex> lock(m_mutex);
				m_data = data;
			}

			T get() {
				std::unique_lock<std::mutex> lock(m_mutex);
				return m_data;
			}

			// This is to avoid lock, get, unlock boilerplate
			void executeAtomicMethod(std::function<void(T &data)> &&method) {
				std::unique_lock<std::mutex> lock(m_mutex);
				method(m_data);
			}

		private:
			T m_data;
			std::mutex m_mutex;
			NotifyPipe<T*> *m_pipe { nullptr };
			IObserver *m_observer {nullptr};
		};
	}
}

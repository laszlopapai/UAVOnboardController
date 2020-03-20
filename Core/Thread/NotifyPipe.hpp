#pragma once

#include <mutex>
#include <functional>
#include "IObserver.h"
#include "IFilter.h"
#include "IObservable.h"
#include "Pipe.h"

namespace IoT {
	namespace Core {

		template<typename T>
		class NotifyPipe
			: IObservable
			, public Pipe<T>
		{

			class DefaultFilter
				: public IFilter<T>
			{
				void filter(T &data) override {}

				~DefaultFilter() = default;
			};

		public:
			explicit NotifyPipe(IFilter<T> *filter = new DefaultFilter)
				: m_filter(filter)
				, m_dataSignal(nullptr)
			{}

			void setFilter(IFilter<T> *filter) {
				delete m_filter;
				m_filter = filter;
			}

			const IFilter<T> *getFilter() const {
				return m_filter;
			}

			virtual ~NotifyPipe() {
				delete m_filter;
			}

			void setObserver(IObserver *observer) override {
				m_dataSignal = observer;
			}

			void pushData(T data, Time time = Time()) override {
				{
					std::unique_lock<std::mutex> lock(m_mutex);
					Pipe<T>::pushData(data, time);
				}

				m_dataSignal->observableChanged();
			}

			bool popData(T *data) override {
				Time time;
				return NotifyPipe<T>::popData(data, &time);
			}

			bool popData(T *data, Time *time) override {
				bool result = false;
				{
					std::unique_lock<std::mutex> lock(m_mutex);
					result = Pipe<T>::popData(data, time);
				}
				m_filter->filter(*data);

				return result;
			}

		private:
			std::mutex m_mutex;
			IFilter<T> *m_filter;
			IObserver *m_dataSignal;
		};
	}
}

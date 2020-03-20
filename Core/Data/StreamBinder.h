#pragma once

#include "StreamPart.h"
#include <bits/algorithmfwd.h>

namespace IoT {
	namespace Core {

		class StreamBinder
			: public StreamPart
		{
		public:
			virtual ~StreamBinder() {
				m_sources.clear();
				m_targets.clear();
			}

			StreamPart& operator|(StreamPart& target) override
			{
				m_targets.push_back(&target);
				for (auto& source : m_sources) {
					target.attach(*source);
				}

				return target;
			}

			StreamPart& operator^(StreamPart& target) override
			{
				m_targets.remove(&target);
				for (auto& source : m_sources) {
					target.detach(*source);
				}
				return target;
			}

			void attach(StreamPart& source) override
			{
				m_sources.push_back(&source);
				for (auto& target : m_targets) {
					target->attach(source);
				}
			}

			void detach(StreamPart& source) override
			{
				m_sources.erase(std::find(m_sources.begin(), m_sources.end(), &source));
				for (auto& target : m_targets) {
					target->detach(source);
				}
			}
		};

	}
}

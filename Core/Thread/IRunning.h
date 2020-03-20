#pragma once

#include <thread>

namespace IoT {
	namespace Core {

		class IRunning
		{
			
		public:
			virtual bool initialize() = 0;

			virtual std::thread *start() = 0;

			virtual void run() = 0;

			virtual void stop() = 0;

			virtual ~IRunning() = default;

		protected:
			virtual void onStop() = 0;

		};
	}
}

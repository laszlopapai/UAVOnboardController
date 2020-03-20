#pragma once

#include "IRunning.h"
#include <vector>

namespace IoT {
	namespace Core {

		class AsyncProcess
			: public IRunning
		{

		public:
			void addSubProcess(AsyncProcess *process);

			std::thread *start() override;

			void stop() final;

			~AsyncProcess() override;

		protected:
			void run() override;

			void onStop() override { }; // This uses Non-Virtual Interface Idiom

			virtual void stopSubproceses();

		private:
			std::thread *m_thread{nullptr};
			std::vector<AsyncProcess*> m_subProcesses;
		};
	}
}

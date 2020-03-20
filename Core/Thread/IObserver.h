#pragma once

namespace IoT {
	namespace Core {

		class IObserver
		{
		public:
			virtual void observableChanged() = 0;

			virtual ~IObserver() = default;
		};
	}
}

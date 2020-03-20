#pragma once

#include "IObserver.h"

namespace IoT {
	namespace Core {

		class IObservable {
		public:
			virtual void setObserver(IObserver *observer) = 0;

			virtual ~IObservable() = default;
		};
	}
}

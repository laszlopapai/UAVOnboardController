#pragma once

namespace IoT {
	namespace Core {

		template<typename T>
		class IFilter
		{
		public:
			virtual void filter(T &data) = 0;

			virtual ~IFilter() = default;
		};
	}
}

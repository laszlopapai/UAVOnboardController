#pragma once

#include <cstdint>
#include <vector>

namespace IoT {
	namespace Core {

		class IPackage {

		public:
			virtual void deserialize(const std::vector<uint8_t> &vector) = 0;

			virtual void serialize(std::vector<uint8_t> &vector) = 0;

			virtual ~IPackage() = default;
		};
	}
}

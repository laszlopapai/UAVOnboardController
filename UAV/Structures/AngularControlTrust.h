#pragma once

#include <Math/Vector3.h>

namespace IoT {
	namespace UAV {

		struct AngularControlTrust
		{
			Core::Vector3 angles;
			float thrust;

			std::string toString() const {
				return angles.toString() + "thrust: " + std::to_string(thrust);
			}
		};

	}
}

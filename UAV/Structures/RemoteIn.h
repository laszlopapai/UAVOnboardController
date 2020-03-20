#pragma once

#include <cstdint>
#include <cstring>
#include <Network/IPackage.h>
#include <Math/Vector3.h>
#include <vector>

namespace IoT {
	namespace UAV {

		enum AircraftAppSignal : uint8_t {
			None       = 0,
			Stop       = 1,
			Reboot     = 2,
			Shutdown   = 4,

			Headlight  = 8,
			Camera     = 16,
			ConfigMode = 32
		};

		class RemoteIn
			: public Core::IPackage
		{
		public:
			void deserialize(const std::vector<uint8_t> &data) override {
				char *rawData = reinterpret_cast<char*>(this) + sizeof(IPackage);
				size_t rawDataLength = sizeof(RemoteIn) - sizeof(IPackage);

				memcpy(rawData, data.data(), rawDataLength);
			}

			void serialize(std::vector<uint8_t> &data) override {}

			Core::Vector3 getSetPoint() const {
				return Core::Vector3(m_roll, m_pitch, m_yaw);
			}

			float getThrust() const {
				return m_thrust;
			}

			uint8_t getRequest() const {
				return m_request;
			}

			bool isSignal(const AircraftAppSignal signal) const {
				return m_request & signal;
			}

			bool isAnySignal() const {
				return m_request != AircraftAppSignal::None;
			}

			AircraftAppSignal getSignal() const {
				return  m_request;
			}

		private:
			float m_yaw		  { 0 };
			float m_pitch		{ 0 };
			float m_roll		 { 0 };
			float m_thrust	   { 0 };

			float m_ledIntensity { 0 };
			AircraftAppSignal m_request { AircraftAppSignal::ConfigMode };
		};
	}
}

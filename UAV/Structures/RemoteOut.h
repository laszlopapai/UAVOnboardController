#pragma once

#include <cstdint>
#include <vector>
#include <cstring>
#include <Math/Vector3.h>
#include <Network/IPackage.h>

namespace IoT {
	namespace UAV {
			
		class RemoteOut
			: public Core::IPackage {

		public:
			void deserialize(const std::vector<uint8_t> &data) override {

			}

			void serialize(std::vector<uint8_t> &data) override {

				size_t length = sizeof(RemoteOut) - sizeof(Core::IPackage);
				data.resize(length);

				auto source = reinterpret_cast<uint8_t *>(this) + sizeof(Core::IPackage);

				memcpy(data.data(), source, length);
			}

		public:
			Core::Vector3 m_angularPosition { Core::Vector3::Zero } ;
			float m_landrange		 { 0 } ;
			float m_altitude		  { 0 } ;
			float m_ascending		 { 0 } ;
			float m_temperature	   { 0 } ;
			float m_pressure		  { 0 } ;
			float m_yawVelocity	   { 0 } ;
			float m_rotorSpeed[4]	 { 0, 0, 0, 0 } ;
		};
	}
}

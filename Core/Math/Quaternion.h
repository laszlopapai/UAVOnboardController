#pragma once


#include <math.h>
#include "Vector3.h"

namespace IoT {
	namespace Core {

		class Quaternion {

		public:
			static const Quaternion Zero;
			static const Quaternion Identity;

		public:
			float w;
			float x;
			float y;
			float z;

		public:
			explicit Quaternion(float w = 1.0f, float x = 0, float y = 0, float z = 0)
				: w(w), x(x), y(y), z(z) {}

			explicit Quaternion(float w, const Vector3 &v)
				: w(w), x(v.x), y(v.y), z(v.z) {}

			Vector3 getAxis() const;

			void fromAngleAxis(const float &rfAngle, const Vector3 &rkAxis);

			Quaternion operator+(const Quaternion &rkQ) const;

			Quaternion operator-(const Quaternion &rkQ) const;

			Quaternion operator*(const Quaternion &rkQ) const;

			Quaternion operator*(float fScalar) const;

			Quaternion friend operator*(float fScalar, const Quaternion &rkQ);

			Vector3 operator*(const Vector3 &v) const;

			Quaternion operator-() const;

			float dot(const Quaternion &rkQ) const;

			float norm() const;

			float normalise(void);

			Quaternion inverse() const;

			Quaternion unitInverse() const;

			Quaternion slerpAngle(float omega, Quaternion end);

			inline std::string toString() const {
				return "(w: " + std::to_string(w) +
						", x: " + std::to_string(x) +
						", y: " + std::to_string(y) +
						", z: " + std::to_string(z) + ")";
			}

			bool isNaN() const {
				return std::isnan(x) || std::isnan(y) || std::isnan(z) || std::isnan(w);
			}
		};
	}
}

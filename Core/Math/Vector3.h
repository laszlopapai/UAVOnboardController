#pragma once

#include <cmath>
#include <cassert>
#include <string>

namespace IoT {
	namespace Core {

		class Vector3 {
		public:
			static const Vector3 Up;
			static const Vector3 Down;
			static const Vector3 Forward;
			static const Vector3 Backward;
			static const Vector3 Left;
			static const Vector3 Right;
			static const Vector3 Zero;
			static const Vector3 One;

			float x;
			float y;
			float z;

			explicit Vector3(float x = 0, float y = 0, float z = 0)
				: x(x)
				, y(y)
				, z(z)
			{}

			inline Vector3 operator+(const Vector3 &rkVector) const {
				return Vector3(
						x + rkVector.x,
						y + rkVector.y,
						z + rkVector.z);
			}

			inline Vector3 operator-(const Vector3 &rkVector) const {
				return Vector3(
						x - rkVector.x,
						y - rkVector.y,
						z - rkVector.z);
			}

			inline Vector3 operator*(const float fScalar) const {
				return Vector3(
						x * fScalar,
						y * fScalar,
						z * fScalar);
			}

			inline Vector3 operator*(const Vector3 &rhs) const {
				return Vector3(
						x * rhs.x,
						y * rhs.y,
						z * rhs.z);
			}

			inline Vector3 operator/(const float fScalar) const {
				assert(fScalar != 0.0);

				float fInv = 1.0f / fScalar;

				return Vector3(
						x * fInv,
						y * fInv,
						z * fInv);
			}

			inline Vector3 operator/(const Vector3 &rhs) const {
				return Vector3(
						x / rhs.x,
						y / rhs.y,
						z / rhs.z);
			}

			inline const Vector3 &operator+() const {
				return *this;
			}

			inline Vector3 operator-() const {
				return Vector3(-x, -y, -z);
			}

			// overloaded operators to help Vector3
			inline friend Vector3 operator*(const float fScalar, const Vector3 &rkVector) {
				return Vector3(
						fScalar * rkVector.x,
						fScalar * rkVector.y,
						fScalar * rkVector.z);
			}

			inline friend Vector3 operator/(const float fScalar, const Vector3 &rkVector) {
				return Vector3(
						fScalar / rkVector.x,
						fScalar / rkVector.y,
						fScalar / rkVector.z);
			}

			inline friend Vector3 operator+(const Vector3 &lhs, const float rhs) {
				return Vector3(
						lhs.x + rhs,
						lhs.y + rhs,
						lhs.z + rhs);
			}

			inline friend Vector3 operator+(const float lhs, const Vector3 &rhs) {
				return Vector3(
						lhs + rhs.x,
						lhs + rhs.y,
						lhs + rhs.z);
			}

			inline friend Vector3 operator-(const Vector3 &lhs, const float rhs) {
				return Vector3(
						lhs.x - rhs,
						lhs.y - rhs,
						lhs.z - rhs);
			}

			inline friend Vector3 operator-(const float lhs, const Vector3 &rhs) {
				return Vector3(
						lhs - rhs.x,
						lhs - rhs.y,
						lhs - rhs.z);
			}

			// arithmetic updates
			inline Vector3 &operator+=(const Vector3 &rkVector) {
				x += rkVector.x;
				y += rkVector.y;
				z += rkVector.z;

				return *this;
			}

			inline Vector3 &operator+=(const float fScalar) {
				x += fScalar;
				y += fScalar;
				z += fScalar;
				return *this;
			}

			inline Vector3 &operator-=(const Vector3 &rkVector) {
				x -= rkVector.x;
				y -= rkVector.y;
				z -= rkVector.z;

				return *this;
			}

			inline Vector3 &operator-=(const float fScalar) {
				x -= fScalar;
				y -= fScalar;
				z -= fScalar;
				return *this;
			}

			inline Vector3 &operator*=(const float fScalar) {
				x *= fScalar;
				y *= fScalar;
				z *= fScalar;
				return *this;
			}

			inline Vector3 &operator*=(const Vector3 &rkVector) {
				x *= rkVector.x;
				y *= rkVector.y;
				z *= rkVector.z;

				return *this;
			}

			inline Vector3 &operator/=(const float fScalar) {
				assert(fScalar != 0.0);

				float fInv = 1.0f / fScalar;

				x *= fInv;
				y *= fInv;
				z *= fInv;

				return *this;
			}

			inline Vector3 &operator/=(const Vector3 &rkVector) {
				x /= rkVector.x;
				y /= rkVector.y;
				z /= rkVector.z;

				return *this;
			}


			/** Returns the length (magnitude) of the vector.
				@warning
					This operation requires a square root and is expensive in
					terms of CPU operations. If you don't need to know the exact
					length (e.g. for just comparing lengths) use squaredLength()
					instead.
			*/
			inline float length() const {
				return sqrtf(x * x + y * y + z * z);
			}

			inline float squaredLength() const {
				return x * x + y * y + z * z;
			}

			inline float distance(const Vector3 &rhs) const {
				return (*this - rhs).length();
			}

			inline float dot(const Vector3 &vec) const {
				return x * vec.x + y * vec.y + z * vec.z;
			}

			inline float normalise() {
				float fLength = sqrtf(x * x + y * y + z * z);

				// Will also work for zero-sized vectors, but will change nothing
				// We're not using epsilons because we don't need to.
				// Read http://www.ogre3d.org/forums/viewtopic.php?f=4&t=61259
				if (fLength > float(0.0f)) {
					float fInvLength = 1.0f / fLength;
					x *= fInvLength;
					y *= fInvLength;
					z *= fInvLength;
				}

				return fLength;
			}

			inline Vector3 cross(const Vector3 &rkVector) const {
				return Vector3(
						y * rkVector.z - z * rkVector.y,
						z * rkVector.x - x * rkVector.z,
						x * rkVector.y - y * rkVector.x);
			}

			inline Vector3 eulerFromDirection(const Core::Vector3& dir, float yaw = 0.0f) const
			{
				/*
				 * float yaw = atan2f(dir.x, dir.y);
				 * float pitch = atan2f(dir.z, sqrtf((dir.x * dir.x) + (dir.y * dir.y)));
				 */
				
				const auto roll = atan2f(dir.z, dir.y) - atan2f(z, y);
				const Vector3 rolledV(x, y * cosf(roll) - z * sinf(roll), y * sinf(roll) + z * cosf(roll));
				auto pitch = acosf(std::min(rolledV.dot(dir), 1.0f));

				const Vector3 x(rolledV.y, rolledV.z, rolledV.x);
				const auto sinPitch = x.cross(rolledV).dot(dir);
				if (sinPitch < 0){
					pitch = -pitch;
				}
				
				return Vector3(roll, pitch, yaw);
			}

			inline std::string toString() const {
				/*char str[100];
				sprintf(str, "(x: %#100.000f, y: %#100.000f, z: %#100.000f)\t", x, y, z);

				return str;*/

				return "(x: " + std::to_string(x) +
						", y: " + std::to_string(y) +
						", z: " + std::to_string(z) + ")";
			}

			inline bool isNaN() const {
				return std::isnan(x) || std::isnan(y) || std::isnan(z);
			}
		};
	}
}

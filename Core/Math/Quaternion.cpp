#include "Quaternion.h"

using namespace IoT::Core;

const Quaternion Quaternion::Zero(0, 0, 0, 0);
const Quaternion Quaternion::Identity(0, 0, 0, 1);

void Quaternion::fromAngleAxis(const float &rfAngle, const Vector3 &rkAxis)
{
	// assert:  axis[] is unit length
	//
	// The quaternion representing the rotation is
	//   q = cos(A/2)+sin(A/2)*(x*i+y*j+z*k)

	float fHalfAngle ( 0.5f * rfAngle );
	float fSin = sinf(fHalfAngle);
	w = cosf(fHalfAngle);
	x = fSin * rkAxis.x;
	y = fSin * rkAxis.y;
	z = fSin * rkAxis.z;
}

Quaternion Quaternion::operator+ (const Quaternion& rkQ) const {
	return Quaternion(w+rkQ.w,x+rkQ.x,y+rkQ.y,z+rkQ.z);
}

Quaternion Quaternion::operator- (const Quaternion& rkQ) const {
	return Quaternion(w-rkQ.w,x-rkQ.x,y-rkQ.y,z-rkQ.z);
}

Quaternion Quaternion::operator* (const Quaternion& rkQ) const {
	// NOTE:  Multiplication is not generally commutative, so in most
	// cases p*q != q*p.

	return Quaternion
			(
					w * rkQ.w - x * rkQ.x - y * rkQ.y - z * rkQ.z,
					w * rkQ.x + x * rkQ.w + y * rkQ.z - z * rkQ.y,
					w * rkQ.y + y * rkQ.w + z * rkQ.x - x * rkQ.z,
					w * rkQ.z + z * rkQ.w + x * rkQ.y - y * rkQ.x
			);
}

Quaternion Quaternion::operator* (float fScalar) const {
	return Quaternion(fScalar*w,fScalar*x,fScalar*y,fScalar*z);
}

Quaternion operator* (float fScalar, const Quaternion& rkQ) {
	return Quaternion(
			fScalar*rkQ.w, fScalar*rkQ.x, fScalar*rkQ.y, fScalar*rkQ.z);
}

Vector3 Quaternion::operator* (const Vector3& v) const
{
	// nVidia SDK implementation
	Vector3 uv, uuv;
	Vector3 qvec(x, y, z);
	uv = qvec.cross(v);
	uuv = qvec.cross(uv);
	uv *= (2.0f * w);
	uuv *= 2.0f;

	return v + uv + uuv;

}

Quaternion Quaternion::operator- () const {
	return Quaternion(-w,-x,-y,-z);
}

float Quaternion::dot(const Quaternion &rkQ) const {
	return w * rkQ.w + x * rkQ.x + y * rkQ.y + z * rkQ.z;
}

/**
 *
 * @return The normal length of the quaternion
 */
float Quaternion::norm() const {
	return w * w + x * x + y * y + z * z;
}

float Quaternion::normalise(void)
{
	float len = norm();
	float factor = 1.0f / sqrtf(len);
	*this = *this * factor;
	return len;
}

Quaternion Quaternion::inverse() const {
	float fNorm = w*w+x*x+y*y+z*z;
	if ( fNorm <= 0.0 ) {
		// return an invalid result to flag the error
		return Quaternion();
	}

	float fInvNorm = 1.0f / fNorm;
	return Quaternion(w*fInvNorm,-x*fInvNorm,-y*fInvNorm,-z*fInvNorm);

}

Quaternion Quaternion::unitInverse() const {
	// assert:  'this' is unit length
	return Quaternion(w,-x,-y,-z);
}

Quaternion Quaternion::slerpAngle(float omega, Quaternion end) {
	Quaternion endAcute = end;
	float dots = dot(end);

	if (dots < 0.0f)
	{
		endAcute = Quaternion(
				-end.w,
				-end.x,
				-end.y,
				-end.z);
	}
	else if (dots > 1.0f)
	{
		dots = 1.0f;
	}

	float theta = acosf(dots);

	if (theta == 0.0f)
	{
		return end;
	}

	float t = omega / theta;
	if (t > 1.0f) t = 1.0f;

	float slerpPitch = sinf((1.0f - t) * theta);
	Quaternion q1 = *this * slerpPitch;

	float slerpRoll = sinf(t * theta);
	Quaternion q2 = endAcute * slerpRoll;

	Quaternion qt = q1 + q2;
	return qt * 1.0f * (1.0f / sinf(theta));
}

Vector3 Quaternion::getAxis() const
{
	float scale = 1.0f / sqrtf(1.0f - (w * w));

	return Vector3(
			x * scale,
			y * scale,
			z * scale
	);
}

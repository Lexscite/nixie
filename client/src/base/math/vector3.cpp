#include "vector3.h"

namespace Nixie
{
	Vector3::Vector3() : x(0), y(0), z(0) {}
	Vector3::Vector3(float value) : x(value), y(value), z(value) {}
	Vector3::Vector3(float x, float y, float z) : x(x), y(y), z(z) {}

	inline float Vector3::GetMagnitude()
	{
		return sqrtf(x * x + y * y + z * z);
	}

	Vector3 Vector3::Normalize()
	{
		float m = GetMagnitude();
		return m == 1 || m == 0 ? *this : Vector3(x / m, y / m, z / m);
	}
}
#include "vec3.h"
inline Vec3 & Vec3::operator+=(const Vec3 & rhs)
{
	e[0] += rhs.e[0];
	e[1] += rhs.e[1];
	e[2] += rhs.e[2];
	return *this;
}

inline Vec3 & Vec3::operator-=(const Vec3 & rhs)
{
	e[0] -= rhs.e[0];
	e[1] -= rhs.e[1];
	e[2] -= rhs.e[2];
	return *this;
}

inline Vec3 & Vec3::operator*=(const Vec3 & rhs)
{
	e[0] *= rhs.e[0];
	e[1] *= rhs.e[1];
	e[2] *= rhs.e[2];
	return *this;
}

inline Vec3 & Vec3::operator/=(const Vec3 & rhs)
{
	e[0] /= rhs.e[0];
	e[1] /= rhs.e[1];
	e[2] /= rhs.e[2];
	return *this;
}

inline Vec3 & Vec3::operator*=(const float t)
{
	e[0] *= t;
	e[1] *= t;
	e[2] *= t;
	return *this;
}

inline Vec3 & Vec3::operator/=(const float k)
{
	float t = 1.0 / k;
	e[0] *= t;
	e[1] *= t;
	e[2] *= t;
	return *this;
}

inline void Vec3::make_unit_vector()
{
	float k = 1.0 / length();
	e[0] *= k;
	e[1] *= k;
	e[2] *= k;
}

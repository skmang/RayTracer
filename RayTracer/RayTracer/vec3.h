#pragma once
#include <math.h>
#include <stdlib.h>
#include <iostream>
class Vec3
{
public:
	Vec3() {}
	Vec3(float e0, float e1, float e2) {
		e[0] = e0;
		e[1] = e1;
		e[2] = e2;
	}

	inline float x() const { return e[0]; }
	inline float y() const { return e[1]; }
	inline float z() const { return e[2]; }
	inline float r() const { return e[0]; }
	inline float g() const { return e[1]; }
	inline float b() const { return e[2]; }

	inline const Vec3& operator+() const { return *this; }
	inline const Vec3 operator-() const { return Vec3(-e[0], -e[1], -e[2]); }
	inline float operator[](int i) const { return e[i]; }
	inline float& operator[](int i)  { return e[i]; }

	inline Vec3& operator+=(const Vec3 &rhs);
	inline Vec3& operator-=(const Vec3 &rhs);
	inline Vec3& operator*=(const Vec3 &rhs);
	inline Vec3& operator/=(const Vec3 &rhs);
	inline Vec3& operator*=(const float t);
	inline Vec3& operator/=(const float t);
	inline void make_unit_vector();

	inline float length() const {
		return sqrt(e[0] * e[0] + e[1] * e[1] + e[2] * e[2]);
	}

	inline float square_length() const {
		return e[0] * e[0] + e[1] * e[1] + e[2] * e[2];
	}

	float e[3];
};

inline std::istream& operator>>(std::istream &is, Vec3& t) {
	is >> t.e[0] >> t.e[1] >> t.e[2];
	return is;
}

inline std::ostream& operator<<(std::ostream &os, const Vec3& t) {
	os << t.e[0] << " " << t.e[1] << " " << t.e[2];
	return os;
}

inline Vec3 operator+(const Vec3& lhs, const Vec3& rhs) {
	return Vec3(lhs.e[0] + rhs.e[0], lhs.e[1] + rhs.e[1], lhs.e[2] + rhs.e[2]);
}

inline Vec3 operator-(const Vec3& lhs, const Vec3& rhs) {
	return Vec3(lhs.e[0] - rhs.e[0], lhs.e[1] - rhs.e[1], lhs.e[2] - rhs.e[2]);
}

inline Vec3 operator*(const Vec3& lhs, const Vec3& rhs) {
	return Vec3(lhs.e[0] * rhs.e[0], lhs.e[1] * rhs.e[1], lhs.e[2] * rhs.e[2]);
}

// 向量除法没意义
inline Vec3 operator/(const Vec3& lhs, const Vec3& rhs) {
	return Vec3(lhs.e[0] / rhs.e[0], lhs.e[1] / rhs.e[1], lhs.e[2] / rhs.e[2]);
}

inline Vec3 operator*(float t, const Vec3& v) {
	return Vec3(t*v.e[0], t*v.e[1], t*v.e[2]);
}

inline Vec3 operator*(const Vec3& v, float t) {
	return Vec3(t*v.e[0], t*v.e[1], t*v.e[2]);
}

inline Vec3 operator/(const Vec3& v, float t) {
	return Vec3(v.e[0] / t, v.e[1] / t, v.e[2] / t);
}

inline float dot(const Vec3& v1, const Vec3& v2) {
	return v1.e[0] * v2.e[0] + v1.e[1] * v2.e[1] + v1.e[2] * v2.e[2];
}

inline Vec3 cross(const Vec3& v1, const Vec3& v2) {
	return Vec3(
		(v1.e[1] * v2.e[2] - v1.e[2] * v2.e[1]),
		(-(v1.e[0] * v2.e[2] - v1.e[2] * v2.e[0])),
		(v1.e[0] * v2.e[1] - v1.e[1] * v2.e[0])
	);
}

inline Vec3 unit_vector(const Vec3& v) {
	return v / v.length();
}
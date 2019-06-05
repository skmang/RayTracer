#pragma once
#include <random>
#include <iostream>
#include "vec3.h"


inline int GetRandomNumber(int min, int max) {
	std::random_device dev;
	std::mt19937 rng(dev());
	std::uniform_int_distribution<std::mt19937::result_type> dist(min, max); // distribution in range [1, 6]
	return dist(rng);
}

inline float GetCanonical() {
	return float(GetRandomNumber(0, 999999)) / 1000000.f;
}

inline Vec3 GetPointInUnitSphere() {
	Vec3 v;
	do {
		v = 2.0*(Vec3(GetCanonical(), GetCanonical(), GetCanonical()) - Vec3(1, 1, 1));
	} while (v.square_length() >= 1.0f);
	return v;
}
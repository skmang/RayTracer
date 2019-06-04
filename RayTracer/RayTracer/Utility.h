#pragma once
#include <limits>
#include <cmath>
#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <chrono>
#include <string>
#include <random>
#include <iomanip>
#include "vec3.h"
#include "Ray.h"
#include "Hitable.h"
#include "HitableList.h"
#include "Sphere.h"
#include "Camera.h"

int GetRandomNumber(int min, int max) {
	std::random_device dev;
	std::mt19937 rng(dev());
	std::uniform_int_distribution<std::mt19937::result_type> dist(min, max); // distribution in range [1, 6]
	return dist(rng);
}

float GetCanonical() {
	return float(GetRandomNumber(0, 999999)) / 1000000.f;
}

Vec3 GetPointInUnitSphere() {
	Vec3 v;
	do {
		v = 2.0*Vec3(GetCanonical(), GetCanonical(), GetCanonical()) - Vec3(1, 1, 1);
	} while (v.square_length() >= 1.0f);
	return v;
}
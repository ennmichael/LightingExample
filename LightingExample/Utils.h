#pragma once

#include <string>
#include <array>
#include <vector>
#include <optional>
#include <cmath>
#include "GL/glew.h"

float constexpr SquareRoot2 = 1.4142135623730951f;

std::string ReadFile(std::string const& fileName);

void CheckGLError();

void EnableBlending();

struct Vector {
	float x;
	float y;

	float Cross(Vector other) const noexcept;

	float DistanceSquared(Vector other) const noexcept;
};

bool operator==(Vector a, Vector b) noexcept;

bool operator!=(Vector a, Vector b) noexcept;

using Polygon = std::vector<Vector>;

template <class Numeric>
int Sign(Numeric n) noexcept
{
	if (n == 0)
		return 0;
	if (n > 0)
		return 1;
	return -1;
}

bool PolarAngleIsLess(Vector a, Vector b, Vector origin) noexcept;

struct Line {
	Vector origin;
	Vector end;

	std::optional<Vector> RayIntersection(Line ray) const noexcept;

	std::optional<Vector> Intersection(Line other) const noexcept;

	float LengthSquared() const noexcept;

	Vector PointWithGreaterPolarAngle(Vector polarOrigin) const noexcept
	{
		if (PolarAngleIsLess(origin, end, polarOrigin))
			return end;
		return origin;
	}

	Vector PointWithSmallerPolarAngle(Vector polarOrigin) const noexcept
	{
		if (PolarAngleIsLess(origin, end, polarOrigin))
			return origin;
		return end;
	}
};

bool operator==(Line a, Line b) noexcept;

bool operator!=(Line a, Line b) noexcept;

struct Dimensions {
	int width;
	int height;
};

struct Circle {
	using OuterDiamond = std::vector<Vector>;

	Vector origin;
	float radius;

	OuterDiamond GetOuterDiamond() const noexcept;
};

void InitGLEW();

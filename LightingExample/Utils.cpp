#include "Utils.h"
#include <fstream>

namespace {

	void ThrowGLEWError(GLenum error)
	{
		std::string errorText;
		for (auto *c = glewGetErrorString(error); *c != '\0'; ++c)
			errorText += *c;
		throw std::runtime_error(errorText);
	}

}

std::string ReadFile(std::string const& fileName)
{
	std::ifstream is(fileName);

	if (!is.good())
		throw std::runtime_error("Error reading file " + fileName);

	std::string content;
	std::string line;
	while (std::getline(is, line))
	{
		content.append(line);
		content.push_back('\n');
	}
	return content;
}

void CheckGLError()
{
	if (auto const error = glGetError(); error != GL_NO_ERROR)
		throw std::runtime_error(reinterpret_cast<char const*>(glewGetErrorString(error)));
}

void EnableBlending()
{
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

float Vector::Cross(Vector other) const noexcept
{
	return x * other.y - other.x * y;
}

float Vector::DistanceSquared(Vector other) const noexcept
{
	float const dx = x - other.x;
	float const dy = y - other.y;
	return dx * dx + dy * dy;
}

bool operator==(Vector a, Vector b) noexcept
{
	return a.x == b.x && a.y == b.y;
}

bool operator!=(Vector a, Vector b) noexcept
{
	return !(a == b);
}

bool PolarAngleIsLess(Vector a, Vector b, Vector origin) noexcept
{
	a = { a.x - origin.x, a.y - origin.y };
	b = { b.x - origin.x, b.y - origin.y };
	return a.Cross(b) > 0.0f;
}

std::optional<Vector> Line::RayIntersection(Line ray) const noexcept
{
	Vector const d1{ end.x - origin.x, end.y - origin.y };
	Vector const d2{ origin.x - ray.origin.x, origin.y - ray.origin.y };
	Vector const d3{ ray.end.x - ray.origin.x, ray.end.y - ray.origin.y };
	float const c = d1.Cross(d3);
	if (std::abs(c) < 1e-6f)
		return std::nullopt;

	float const t = d1.Cross(d2) / c;
	if (t < 0.0f)
		return std::nullopt;

	float const u = (ray.origin.Cross(origin) + d3.Cross(origin) * t) / d1.Cross(origin);
	if (u < 0.0f || u > 1.0f)
		return std::nullopt;

	return Vector{ ray.origin.x + t * d3.x, ray.origin.y + t * d3.y };
}

std::optional<Vector> Line::Intersection(Line other) const noexcept
{
	Vector const d1{ end.x - origin.x, end.y - origin.y };
	Vector const d2{ origin.x - other.origin.x, origin.y - other.origin.y };
	Vector const d3{ other.end.x - other.origin.x, other.end.y - other.origin.y };
	float const c = d1.Cross(d3);
	if (std::abs(c) < 1e-6f)
		return std::nullopt;

	float const t = d1.Cross(d2) / c;
	if (t < 0.0f || t > 1.0f)
		return std::nullopt;

	Vector const d4{ other.origin.x - origin.x, other.origin.y - origin.y };
	float const u = d4.Cross(d3) / c;
	if (u < 0.0f || u > 1.0f)
		return std::nullopt;

	return Vector{ other.origin.x + t * d3.x, other.origin.y + t * d3.y };
}

float Line::LengthSquared() const noexcept
{
	return origin.DistanceSquared(end);
}

bool operator==(Line a, Line b) noexcept
{
	return a.origin == b.origin && a.end == b.end;
}

bool operator!=(Line a, Line b) noexcept
{
	return !(a == b);
}

auto Circle::GetOuterDiamond() const noexcept -> OuterDiamond
{
	auto const k = radius * SquareRoot2;
	return {
		Vector{ origin.x, origin.y + k },
		Vector{ origin.x + k, origin.y },
		Vector{ origin.x, origin.y - k },
		Vector{ origin.x - k, origin.y },
	};
}

void InitGLEW()
{
	if (auto const error = glewInit(); error != GLEW_OK)
		ThrowGLEWError(error);
}

#pragma once

#include <vector>
#include <string>
#include <optional>
#include <deque>
#include "GL/glew.h"
#include "SDL.h"
#include "Buffers.h"
#include "Utils.h"
#include "Program.h"

#include <iostream>

using namespace std::string_literals;

using Terrain = std::vector<Line>;

float constexpr pi = 3.1415926535897932f;

struct Uniforms {
	GLint radiusSquared;
	GLint maxAlpha;
	GLint origin;
	GLint drawingLighting;
	GLint color;
};

template <class Bound>
void InsertClockwise(std::deque<Vector>& sortedPolygon, Vector newPoint, Vector origin, Bound const& bound)
{
	auto const i = bound(sortedPolygon.begin(), sortedPolygon.end(), newPoint,
		[&](auto const v1, auto const v2)
		{
			return PolarAngleIsLess(v1, v2, origin);
		}
	);
	sortedPolygon.insert(i, newPoint);
}

void ClockwiseSortTerrain(Terrain& terrain, Vector origin);

std::optional<Vector> ClosestRayIntersection(Line ray, Line currentLine, Terrain const& terrain) noexcept;

class Light {
public:
	Light(Circle circle, float intensity)
		: circle(circle)
		, intensity(intensity)
	{
		if (intensity < 0.0f || intensity > 1.0f)
			throw std::runtime_error("Invalid intensity");
	}

	void Draw(Uniforms uniforms, Terrain const& terrain) const;

	void DrawPolygon(Uniforms uniforms, Terrain& terrain) const
	{
		glUniform4f(uniforms.color, 1.0f, 0.0f, 0.0f, 0.2f);
		glUniform1i(uniforms.drawingLighting, 0);
		auto const polygon = GetPolygon(terrain);
		BufferData(GL_ARRAY_BUFFER, polygon, GL_STATIC_DRAW);
		glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, nullptr);
		// glDrawArrays(GL_TRIANGLE_FAN, 0, polygon.size());
		glPointSize(5.0);
		glDrawArrays(GL_POINTS, 0, polygon.size());
		glEnable(GL_PROGRAM_POINT_SIZE);
		glPointSize(1.0);
		glUniform4f(uniforms.color, 0.0f, 0.0f, 1.0f, 0.5f);
		/*auto const points = circle.GetOuterDiamond();
		BufferData(GL_ARRAY_BUFFER, points, GL_STATIC_DRAW);
		glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, nullptr);
		glDrawArrays(GL_LINE_LOOP, 0, points.size());*/
	}

	void SetOrigin(Vector origin)
	{
		circle.origin = origin;
	}

	Polygon GetPolygon(Terrain const& terrain) const;

private:
	Circle circle;
	float intensity;
};

class LightSource {
public:
	LightSource(Vector origin, Uniforms const& uniforms)
		: innerLight(Circle{ origin, 0.25f }, 0.4f)
		, outerLight(Circle{ origin, 0.5f }, 0.25f)
		, uniforms(uniforms)
	{}

	void Draw(Terrain& terrain)
	{
		// innerLight.Draw(uniforms, terrain);
		outerLight.Draw(uniforms, terrain);
	}

	void SetOrigin(Vector origin)
	{
		innerLight.SetOrigin(origin);
		outerLight.SetOrigin(origin);
	}

private:
	Light innerLight;
	Light outerLight;
	Uniforms uniforms;
};

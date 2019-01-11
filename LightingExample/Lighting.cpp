#include "Lighting.h"
#include "SDL.h"
#include <algorithm>
#include <cassert>
#include <cmath>

void ClockwiseSortTerrain(Terrain& terrain, Vector origin)
{
	std::sort(terrain.begin(), terrain.end(),
		[&](auto const line1, auto const line2)
	{
		Vector const a = line1.PointWithGreaterPolarAngle(origin);
		Vector const b = line2.PointWithGreaterPolarAngle(origin);
		return a.Cross(b) < 0.0f;
	}
	);
}

// TODO Check this
std::optional<Vector> ClosestLineIntersection(Line line, Terrain const& terrain) noexcept
{
	std::optional<Vector> result = std::nullopt;

	for (auto const& terrainLine : terrain)
	{
		std::optional<Vector> const intersection = line.Intersection(terrainLine);
		if (intersection == std::nullopt)
			continue;

		if (result == std::nullopt || line.origin.DistanceSquared(*intersection) < line.origin.DistanceSquared(*result))
			result = intersection;
	}

	return result;
}

void Light::Draw(Uniforms uniforms, Terrain const& terrain) const
{
	glUniform1i(uniforms.drawingLighting, 1);
	glUniform1f(uniforms.radiusSquared, circle.radius * circle.radius);
	glUniform1f(uniforms.maxAlpha, intensity);
	glUniform2f(uniforms.origin, circle.origin.x, circle.origin.y);

	auto const polygon = GetPolygon(terrain);

	glUniform1i(uniforms.drawingLighting, 1);
	BufferData(GL_ARRAY_BUFFER, polygon, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, nullptr);
	glDrawArrays(GL_TRIANGLE_FAN, 0, polygon.size());

	/*
	glUniform1i(uniforms.drawingLighting, 0);
	glUniform4f(uniforms.color, 1.0f, 0.0f, 0.0f, 0.5f);
	glPointSize(5.0f);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, nullptr);
	glDrawArrays(GL_POINTS, 0, polygon.size());

	glUniform4f(uniforms.color, 0.0f, 1.0f, 0.0f, 0.5f);
	BufferData(GL_ARRAY_BUFFER, terrain, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, nullptr);
	glDrawArrays(GL_LINES, 0, 2 * terrain.size());
	*/
}

Polygon Light::GetPolygon(Terrain const& terrain) const
{
	// There's a better way, drawing the outer diamond of the circle and then looking for intersections and stuff.
	// I have this in one of my notebooks. It's a lot more complex to implement though.

	// There's also the possibility of drawing the circle using Brensenham's algorithm. These aren't integers however, they are
	// floats, so I have no idea what that would look like.

	float constexpr step = 0.01f;

	Polygon polygon{ circle.origin };

	auto const findEnd = [&](Vector circlePoint)
	{
		std::optional<Vector> const intersection = ClosestLineIntersection(Line{ circle.origin, circlePoint }, terrain);
		if (intersection == std::nullopt)
			return circlePoint;
		return *intersection;
	};

	for (float alpha = 0; alpha < 2 * pi; alpha += step)
	{
		Vector const end = findEnd({ circle.origin.x + circle.radius * std::cos(alpha), circle.origin.y + circle.radius * std::sin(alpha) });
		polygon.push_back(end);
	}

	polygon.push_back(polygon.at(0));

	return polygon;
}

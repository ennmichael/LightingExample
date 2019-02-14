#version 440 core

layout (location = 0) out vec4 OutputColor;

in vec4 Vertex;

uniform vec2 Origin;

uniform float RadiusSquared;

uniform float MaxAlpha;

uniform bool DrawingLighting;

uniform vec4 Color;

float DistanceSquared()
{
	float x = abs(Vertex.x - Origin.x);
	float y = abs(Vertex.y - Origin.y);
	return x * x + y * y;
}

float Alpha()
{
	float k = DistanceSquared() / RadiusSquared;
	return MaxAlpha - k * MaxAlpha;
}

void main()
{
	if (DrawingLighting)	
		OutputColor = vec4(1.0, 1.0, 1.0, Alpha());
	else
		OutputColor = Color;
}

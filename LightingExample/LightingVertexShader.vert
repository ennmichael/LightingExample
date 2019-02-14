#version 440 core

layout (location = 0) in vec4 InputVertex;

out vec4 Vertex;

void main()
{
	gl_Position = InputVertex;
	Vertex = InputVertex;
}

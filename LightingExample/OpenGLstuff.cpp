#include <iostream>
#include <memory>
#include <stdexcept>
#include <cstddef>
#include <array>
#include <limits>
#include <random>
#include "GL/glew.h"
#include "SDL.h"
#include "Utils.h"
#include "Program.h"
#include "Buffers.h"
#include "SDLUtils.h"
#include "Lighting.h"

using namespace std::string_literals;

class LightingExample {
public:
	LightingExample()
		: window(CreateWindow("Title"s, 500, 500))
		, glContext(CreateGLContext(*window))
		, program({
			ShaderFile{"LightingVertexShader.vert", GL_VERTEX_SHADER},
			ShaderFile{"LightingFragmentShader.frag", GL_FRAGMENT_SHADER},
		})
		, uniforms(Uniforms{
			program.GetUniformLocation("RadiusSquared"s),
			program.GetUniformLocation("MaxAlpha"s),
			program.GetUniformLocation("Origin"s),
			program.GetUniformLocation("DrawingLighting"s),
			program.GetUniformLocation("Color"s),
		})
		, buffers(Buffers::ForTargets({ GL_ARRAY_BUFFER }))
		, lightSource(Vector{ 0.0f, 0.0f }, uniforms)
	{
		EnableBlending();
		SDL_GL_SetSwapInterval(1);
		program.Use();
		glEnableVertexAttribArray(0);
	}

	void MainLoop()
	{
		while (!SDL_QuitRequested())
		{
			lightSource.SetOrigin(GetMousePosition(*window));
			Redraw();
		}
	}

	void Redraw()
	{
		ClearBackground();
		lightSource.Draw(terrain);
		DrawTerrain();
		SDL_GL_SwapWindow(window.get());
	}

	void DrawTerrain() const
	{
		glUniform1i(uniforms.drawingLighting, 0);
		glUniform4f(uniforms.color, 0.0f, 1.0f, 0.0f, 0.5f);
		BufferData(GL_ARRAY_BUFFER, terrain, GL_STATIC_DRAW);
		glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, nullptr);
		glDrawArrays(GL_LINES, 0, 2 * terrain.size());
	}

	void ClearBackground()
	{
		std::array constexpr black{ 0.0f, 0.0f, 0.0f, 1.0f };
		glClearBufferfv(GL_COLOR, 0, black.data());
	}

private:
	UniqueWindow window;
	UniqueGLContext glContext;
	Program program;
	Uniforms uniforms;
	Buffers buffers;
	LightSource lightSource;
	Terrain terrain{
		Line{ Vector{ 0.2f, -0.15f }, Vector{ 0.4f, -0.05f } },
		Line{ Vector{ 0.1f, 0.3f }, Vector{ 0.1f, -0.05f } },
		Line{ Vector{ -0.35f, 0.7f }, Vector{ -0.35f, -0.6f } },
	};
};

int main(int, char**)
{
	std::cout << PolarAngleIsLess(Vector{ 1.0f, 1.0f }, Vector{ 1.0f, -1.0f }, Vector{ 0.0f, 0.0f }) << '\n';
	std::cout << PolarAngleIsLess(Vector{ 1.0f, -1.0f }, Vector{ 1.0f, 1.0f }, Vector{ 0.0f, 0.0f }) << '\n';
	LightingExample lightingExample;
	lightingExample.MainLoop();
	return 0;
}

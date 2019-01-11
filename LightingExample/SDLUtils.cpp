#include "SDLUtils.h"
#include "Utils.h"

namespace {

void ThrowSDLError()
{
	throw std::runtime_error(SDL_GetError());
}

}

UniqueWindow CreateWindow(std::string const& title, int width, int height)
{
	UniqueWindow window(SDL_CreateWindow(
		title.c_str(), SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, width, height, SDL_WINDOW_OPENGL));
	if (window == nullptr)
		ThrowSDLError();
	return window;
}

UniqueGLContext CreateGLContext(SDL_Window& window)
{
	UniqueGLContext context(SDL_GL_CreateContext(&window));
	if (context == nullptr)
		ThrowSDLError();
	InitGLEW();
	return context;
}

Dimensions GetWindowDimensions(SDL_Window& window)
{
	Dimensions dimensions;
	SDL_GetWindowSize(&window, &dimensions.width, &dimensions.height);
	return dimensions;
}

SDLGuard::SDLGuard()
{
	if (SDL_Init(SDL_INIT_EVERYTHING) < 0)
		ThrowSDLError();
}

SDLGuard::~SDLGuard()
{
	SDL_Quit();
}

Vector GetMousePosition(SDL_Window& window)
{
	// THIS IS CODE FOR CONVERTING TO NDC, WILL NEED IT LATER
	int x;
	int y;
	if (SDL_GetMouseState(&x, &y) < 0)
		ThrowSDLError();
	auto const windowDimensions = GetWindowDimensions(window);
	float widthHalf = static_cast<float>(windowDimensions.width) / 2.0f;
	float heightHalf = static_cast<float>(windowDimensions.height) / 2.0f;
	return Vector{
		static_cast<float>(x) / widthHalf - 1.0f,
		-static_cast<float>(y) / heightHalf + 1.0f,
	};
}

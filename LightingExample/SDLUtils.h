#pragma once

#include <memory>
#include <string>
#include "SDL.h"
#include "Utils.h"

template <auto Func>
struct FuncDeleter {
	template <class T>
	void operator()(T const& resource)
	{
		Func(resource);
	}
};

using UniqueWindow = std::unique_ptr<SDL_Window, FuncDeleter<SDL_DestroyWindow>>;

using UniqueGLContext = std::unique_ptr<void, FuncDeleter<SDL_GL_DeleteContext>>;

UniqueWindow CreateWindow(std::string const& title, int width, int hegiht);

UniqueGLContext CreateGLContext(SDL_Window& window);

Dimensions GetWindowDimensions(SDL_Window& window);

struct SDLGuard {
	SDLGuard();

	~SDLGuard();

	SDLGuard(SDLGuard const& other) = delete;

	SDLGuard(SDLGuard&& other) = delete;

	SDLGuard& operator=(SDLGuard const& other) = delete;

	SDLGuard& operator=(SDLGuard&& other) = delete;
};

Vector GetMousePosition(SDL_Window& window);

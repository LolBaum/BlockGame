#ifndef SDL_HANDLER_H
#define SDL_HANDLER_H

#pragma once
#define SDL_MAIN_HANDLED
#include <SDL.h>
#include <iostream>
#include <sstream>

#include "definitions.hpp"
#include "version.hpp"

class SDL_handler {
public:
	static void initialize();

	static void SDL_SwapWindow();

	static void SetRelativeMouseMode(bool b);

	static int getWidth();
	static int getHeight();
	static float getAspectRatio();
	static SDL_Window* getWindow();

	static void SetWindowSize_ToDisplay();

private:
	static SDL_Window* window;
	static int width;
	static int height;
	SDL_handler();
	~SDL_handler();
}; 



#endif
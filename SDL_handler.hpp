#pragma once
#define SDL_MAIN_HANDLED
#include <SDL.h>
#include <iostream>
#include <sstream>

#include "definitions.hpp"
#include "version.hpp"

class SDL_handler {
public:
	SDL_handler(){}
	~SDL_handler() {}
	

	void initialize() {
		std::cout << "Initializing the SDL" << std::endl;

		SDL_Init(SDL_INIT_EVERYTHING);

		SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8);
		SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
		SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);
		SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, 8);
		SDL_GL_SetAttribute(SDL_GL_BUFFER_SIZE, 32);
		SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

		SDL_GL_SetSwapInterval(1); // Vsync 

#ifdef _DEBUG
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, SDL_GL_CONTEXT_DEBUG_FLAG);
#endif

		uint32 flags = SDL_WINDOW_OPENGL;

		std::stringstream Window_title;
		Window_title << GAME_NAME << "   version: " << GAME_VERSION_MAJOR << "." << GAME_VERSION_MINOR << std::endl;

		window = SDL_CreateWindow(Window_title.str().c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, flags);
		SDL_GLContext glContext = SDL_GL_CreateContext(window);
	}

	void SDL_SwapWindow() {
		SDL_GL_SwapWindow(window);
	}

	void SetRelativeMouseMode(bool b) {
		if (b) {
			SDL_SetRelativeMouseMode(SDL_TRUE);
		}
		else {
			SDL_SetRelativeMouseMode(SDL_FALSE);
		}
	}

	int getWidth() {
		return width;
	}
	int getHeight() {
		return height;
	}
	SDL_Window* getWindow(){
		return window;
	}

private:
	SDL_Window* window;
	int width = 1440;
	int height = 900;
}; 

SDL_handler sdl_handler = SDL_handler();
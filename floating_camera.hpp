#pragma once
#include "fps_camera.hpp"
#include "SDL_handler.hpp"

class FloatingCamera : public FPSCamera {
public:
	FloatingCamera(float fov, float width, float height) : FPSCamera(fov, width, height) {

	}

	FloatingCamera() : FPSCamera(91.5f, (float32) sdl_handler.getWidth(), (float32)sdl_handler.getHeight()) {

	}

	~FloatingCamera(){}

	void moveUp(float amount) {
		translate(up * amount);
	}

};

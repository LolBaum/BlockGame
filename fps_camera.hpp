#pragma once
#include "camera.hpp"

class FPSCamera : public Camera {
public:
	FPSCamera(float fov, float width, float height, float cam_height = 1.7f) : Camera(fov, width, height){
		up = glm::vec3(0.0f, 1.0f, 0.0f);
		camera_height = glm::vec3(0.0f, cam_height, 0.0f);
		yaw = -90.0f;
		pitch = 0.0f;
		onMouseMoved(0.0f, 0.0f);
		update();
	}

	void onMouseMoved(float xRel, float yRel) {
		yaw += xRel * mouseSentitivity;
		pitch -= yRel * mouseSentitivity;
		if (pitch > 89.0f)
			pitch = 89.0f;
		if (pitch < -89.0f)
			pitch = -89.0f;

		glm::vec3 front;
		front.x = cos(glm::radians(pitch)) * cos(glm::radians(yaw));
		front.y = sin(glm::radians(pitch));
		front.z = cos(glm::radians(pitch)) * sin(glm::radians(yaw));
		lookAt = glm::normalize(front);
		update();
	}

/* 	void update() override {
		view = glm::lookAt(position, position + lookAt, up);
		viewProj = projection * view;
	} */

	void update() override {
		view = glm::lookAt(position+camera_height, position+camera_height + lookAt, up);
		viewProj = projection * view;
	}


	void moveFront(float amount) {
		translate(glm::normalize(glm::vec3(1.0f, 0.0f, 1.0f) * lookAt) * amount);
		update();
	}
	
	void moveSideways(float amount) {
		translate(glm::normalize(glm::cross(lookAt, up)) * amount);
		update();
	}

	glm::vec3 getLookAt() {
		return lookAt;
	}

	glm::vec3 getViewPos() {
		return position+camera_height;
	}

	void set_camera_height(float height, float offset=-0.15){
		camera_height = glm::vec3(0.0f, height + offset, 0.0f);
	}

protected:
	float yaw;
	float pitch;
	glm::vec3 lookAt;
	const float mouseSentitivity = 0.3f;
	glm::vec3 up;
	glm::vec3 camera_height;
};
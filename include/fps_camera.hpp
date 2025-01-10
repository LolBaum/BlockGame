#pragma once
#include "camera.hpp"

class FPSCamera : public Camera {
public:
	FPSCamera(float fov, float width, float height, float cam_height=1.7f) : Camera(fov, width, height){
		up = glm::vec3(0.0f, 1.0f, 0.0f);
		camera_height = glm::vec3(0.0f, cam_height, 0.0f);
		yaw = -90.0f;
		pitch = 0.0f;
		lookAt = {0, 0, 1};
		update();
	}


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
    void setLookAt(glm::vec3 v){
        lookAt = v;
    }
	void setPitchYaw(float p, float yw) {
		pitch = p;
		yaw = yw;
	}
	glm::vec3 getRotation(){
		return glm::vec3(yaw, pitch, 0.0f);
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
	glm::vec3 up;
	glm::vec3 camera_height;
};
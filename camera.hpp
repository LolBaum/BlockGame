#pragma once

#include "glm/glm.hpp"
#include "glm/ext/matrix_transform.hpp"
#include "glm/gtc/matrix_transform.hpp"

class Camera {
public:

	Camera(float fov, float width, float height) {
		projection = glm::perspective(fov / 2.0f, width / height, 0.1f, 1000.0f);
		view = glm::mat4(1.0f);
		position = glm::vec3(0.0f);
		update();

		this->width = width;
		this->height = height;
	}

	glm::mat4 getViewProj() {
		return viewProj;
	}

	virtual void update() {
		viewProj = projection * view;
	}

	virtual void translate(glm::vec3 v) {
		position += v;
		view = glm::translate(view, v * -1.0f);
	}

	void setFov(float newFov) {
		projection = glm::perspective(newFov / 2.0f, width / height, 0.1f, 1000.0f);
	}

	glm::vec3 getPos() {
		return position;
	}


protected:
	glm::vec3 position;
	glm::mat4 projection;
	glm::mat4 view;
	glm::mat4 viewProj;
	float32 width;
	float32 height;
};
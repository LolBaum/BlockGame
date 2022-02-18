#pragma once
#include <glew.h>

#include "definitions.h"

struct IndexBuffer {
	IndexBuffer() {
		glGenBuffers(1, &bufferId);
	}
	IndexBuffer(void* data, uint32 numIndices, uint8 elementSize) {
		glGenBuffers(1, &bufferId);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, bufferId);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, elementSize * numIndices, data, GL_STATIC_DRAW);
	}
	~IndexBuffer(){
		glDeleteBuffers(1, &bufferId);
	}

	void update(void* data, uint32 numIndices, uint8 elementSize) {
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, bufferId);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, elementSize * numIndices, data, GL_STATIC_DRAW);
	}

	void bind(){
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, bufferId);
	}

	void unbind(){
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	}

private:
	GLuint bufferId;
};
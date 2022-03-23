#pragma once
#include <glew.h>

#include "definitions.hpp"
#include <iostream>

struct ChunkVertexBuffer {
	ChunkVertexBuffer() {
		glGenVertexArrays(1, &vao);
		glBindVertexArray(vao);

		glGenBuffers(1, &bufferId);
		glBindVertexArray(0);
	}
	ChunkVertexBuffer(void* data, int numVertices) {
		glGenVertexArrays(1, &vao);
		glBindVertexArray(vao);

		glGenBuffers(1, &bufferId);
		glBindBuffer(GL_ARRAY_BUFFER, bufferId);
		glBufferData(GL_ARRAY_BUFFER, numVertices * sizeof(Vertex), data, GL_STATIC_DRAW);

		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(struct Vertex, x));
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(struct Vertex, u));
		glEnableVertexAttribArray(2);
		glVertexAttribPointer(2, 1, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(struct Vertex, i));

		glBindVertexArray(0);
	}

	void update(void* data, int numVertices) {
		glBindVertexArray(vao);
		glBindBuffer(GL_ARRAY_BUFFER, bufferId);
		glBufferData(GL_ARRAY_BUFFER, numVertices * sizeof(Vertex), data, GL_STATIC_DRAW);

		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(struct Vertex, x));
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(struct Vertex, u));
		glEnableVertexAttribArray(2);
		glVertexAttribPointer(2, 1, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(struct Vertex, i));

		glBindVertexArray(0);
	}

	~ChunkVertexBuffer() {
		glDeleteBuffers(1, &bufferId);
	}

	void bind() {
		glBindVertexArray(vao);
	}

	void unbind() {
		glBindVertexArray(0);
	}

private:
	GLuint bufferId;
	GLuint vao;
};


struct Vertex2DBuffer {
	Vertex2DBuffer() {
		glGenVertexArrays(1, &vao);
		glBindVertexArray(vao);

		glGenBuffers(1, &bufferId);
		glBindVertexArray(0);
	}
	Vertex2DBuffer(void* data, int numVertices) {
		glGenVertexArrays(1, &vao);
		glBindVertexArray(vao);

		glGenBuffers(1, &bufferId);
		glBindBuffer(GL_ARRAY_BUFFER, bufferId);
		glBufferData(GL_ARRAY_BUFFER, numVertices * sizeof(Vertex2D), data, GL_STATIC_DRAW);

		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex2D), (void*)offsetof(struct Vertex, x));
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex2D), (void*)offsetof(struct Vertex, u));

		glBindVertexArray(0);
	}

	void update(void* data, int numVertices) {
		glBindVertexArray(vao);
		glBindBuffer(GL_ARRAY_BUFFER, bufferId);
		glBufferData(GL_ARRAY_BUFFER, numVertices * sizeof(Vertex2D), data, GL_STATIC_DRAW);

		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex2D), (void*)offsetof(struct Vertex, x));
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex2D), (void*)offsetof(struct Vertex, u));

		glBindVertexArray(0);
	}

	~Vertex2DBuffer() {
		glDeleteBuffers(1, &bufferId);
	}

	void bind() {
		glBindVertexArray(vao);
	}

	void unbind() {
		glBindVertexArray(0);
	}

private:
	GLuint bufferId;
	GLuint vao;
};



struct ColorVertexBuffer {
	ColorVertexBuffer(void* data, int numVertices){
		glGenVertexArrays(1, &vao);
		glBindVertexArray(vao);

		glGenBuffers(1, &bufferId);
		glBindBuffer(GL_ARRAY_BUFFER, bufferId);
		glBufferData(GL_ARRAY_BUFFER, numVertices * sizeof(ColorVertex), data, GL_STATIC_DRAW);

		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(ColorVertex), (void*)offsetof(struct ColorVertex, x));
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(ColorVertex), (void*)offsetof(struct ColorVertex, u));
		glEnableVertexAttribArray(2);
		glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, sizeof(ColorVertex), (void*)offsetof(struct ColorVertex, r));

		glBindVertexArray(0);
	}

	~ColorVertexBuffer(){
		glDeleteBuffers(1, &bufferId);
	}

	void bind(){
		glBindVertexArray(vao);
	}

	void unbind(){
		glBindVertexArray(0);
	}

private:
	GLuint bufferId;
	GLuint vao;
};
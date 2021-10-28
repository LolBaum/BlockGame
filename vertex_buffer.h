#pragma once
#include <GL/glew.h>

#include "definitions.h"

struct ChunkVertexBuffer {
	ChunkVertexBuffer() {
		glGenVertexArrays(1, &vao);
		glBindVertexArray(vao);

		glGenBuffers(1, &bufferId);
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
		/*glGenVertexArrays(1, &vao);
		glBindVertexArray(vao);

		glGenBuffers(1, &bufferId);*/
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
		//std::cout << "Called Destructor VertexBuffer: " << bufferId << std::endl;
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
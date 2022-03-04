#pragma once
#include <glew.h>
#include <string>
#include "definitions.hpp"

struct Shader {
	Shader(const char* ColorVertexShaderFilename, const char* fragmentShaderFilename);
	virtual ~Shader();

	void bind();
	void unbind();

	GLuint getShaderId() {
		return shaderId;
	}

private:

	GLuint compile(std::string shaderSource, GLenum type);
	std::string parse(const char* filename);
	GLuint createShader(const char* ColorVertexShaderFilename, const char* fragmentShaderFilename);

	GLuint shaderId;
};
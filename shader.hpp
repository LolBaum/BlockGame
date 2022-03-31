#pragma once
#include <glew.h>
#include <string>
#include "definitions.hpp"
#include "glm/glm.hpp"

struct Shader {
	Shader(const char* ColorVertexShaderFilename, const char* fragmentShaderFilename);
	Shader();
	void initialize(const char* ColorVertexShaderFilename, const char* fragmentShaderFilename);
	virtual ~Shader();

	void bind();
	void unbind();

	GLuint getShaderId() {
		return shaderId;
	}

	void setVec2(const std::string &name, const glm::vec2 &value) const;
	void setVec2(const char*, const glm::vec2 &value) const;

private:

	GLuint compile(std::string shaderSource, GLenum type, std::string name);
	std::string parse(const char* filename);
	GLuint createShader(const char* ColorVertexShaderFilename, const char* fragmentShaderFilename);
	

	GLuint shaderId;
};
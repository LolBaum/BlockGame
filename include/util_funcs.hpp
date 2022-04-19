#pragma once

#ifndef UTIL_FUNCS_H
#define UTIL_FUNCS_H

#include <glew.h>
#include <iostream>
#include <cmath>

#include "glm/glm.hpp"

#define CX 16
#define CY 16
#define CZ 16

#ifdef _DEBUG

void _GLGetError(const char* file, int line, const char* call) {
	while (GLenum error = glGetError()) {
		std::cout << "[OpenGL Error] " << glewGetErrorString(error) << " in " << file << ":" << line << " Call: " << call << std::endl;
	}
}

#define GLCALL(call) call; _GLGetError(__FILE__, __LINE__, #call)

#else

#define GLCALL(call) call

#endif


bool exists (const std::string& name);

bool isDir(const std::string& name);

void createDir(const std::string& name);

void createDir_IfDoesNotExist(const std::string& name);


template<typename T>
int sign(T Value){
	if (Value>=0){
		return 1;
	}
	else{
		return -1;
	}
}


bool compareVec3(glm::vec3 v1, glm::vec3 v2);

bool isChunkEmty(glm::vec3 v1);
glm::vec3 Chunk_Position(glm::vec3 pos);
glm::vec3 position_in_chunk(glm::vec3 pos);

glm::vec3 scale_vec3(glm::vec3 vec, float value);
bool is_inside_block(glm::vec3 blockpos, glm::vec3 pos);

#endif 
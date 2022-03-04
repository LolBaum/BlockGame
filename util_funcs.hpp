#pragma once

#include <glew.h>
#include <iostream>

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


template<typename T>
int sign(T Value){
	if (Value>=0){
		return 1;
	}
	else{
		return -1;
	}
}



bool compareVec3(glm::vec3 v1, glm::vec3 v2) {
	return ((v1.x == v2.x) && (v1.y == v2.y) && (v1.z == v2.z));
}

bool isChunkEmty(glm::vec3 v1) {
	return ((v1.x == -1) && (v1.y == -1) && (v1.z == -1));
}

glm::vec3 Chunk_Position(glm::vec3 pos) {
	glm::vec3 chunkpos;
	chunkpos.x = floor(pos.x / 16) * 16;
	chunkpos.y = floor(pos.y / 16) * 16;
	chunkpos.z = floor(pos.z / 16) * 16;
	return chunkpos;
}
glm::vec3 position_in_chunk(glm::vec3 pos) {
	int x;
	if (pos.x >= 0) {
		x = abs((int)pos.x % CX); // not floor(). use (int)
	}
	else {
		x = CX -1 - abs((int)pos.x % CX);
	}

	int y;
	if (pos.y >= 0) {
		y = abs((int)pos.y % CY);
	}
	else {
		y = CY -1 - abs((int)pos.y % CY);
	}

	int z;
	if (pos.z >= 0) {
		z = abs((int)pos.z % CZ);
	}
	else {
		z = CZ -1 - abs((int)pos.z % CZ);
	}
	return glm::vec3(x, y, z);
}

glm::vec3 scale_vec3(glm::vec3 vec, float value) {
	vec.x *= value;
	vec.y *= value;
	vec.z *= value;
	return vec;
}

//glm::vec3& operator*(glm::vec3& vec, const int& value)
//{
//	vec.x *= value;
//	vec.y *= value;
//	vec.z *= value;
//	return vec;
//}
//glm::vec3& operator*(glm::vec3& vec, const float& value)
//{
//	vec.x *= value;
//	vec.y *= value;
//	vec.z *= value;
//	return vec;
//}
//glm::vec3& operator*(glm::vec3& vec, const double& value)
//{
//	vec.x *= value;
//	vec.y *= value;
//	vec.z *= value;
//	return vec;
//}
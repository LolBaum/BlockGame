#pragma once
#ifndef PRINT_FUNCTIONS_H
#define PRINT_FUNCTIONS_H

#include <iostream>
#include <ostream>
#include <vector>
#ifdef _WIN32
#include <windows.h>
#include <conio.h>
#endif
#include <string.h>
#include <sstream>
#include <ctime>
#include <iomanip>
#include "glm/glm.hpp"
#include "glew.h"

using namespace std;

enum Level {
	debug, error, warning, info
};

void Set_Console_Color(int color);

void print_warning(string output);

void print_error(string output);
void print_Debug(string output, int format=0);

void openGLDebugCallback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, const void* userParam);






void slowPrint(string output);

std::string vec3_toString(glm::vec3 vec, std::string prefix = "",int precision=5);

std::string posVector_toString(std::vector<glm::vec3> vector, std::string prefix = "");

#endif 

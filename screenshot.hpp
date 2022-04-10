#pragma once
#include <string>
#include <glew.h>

#include <sys/stat.h>
inline bool exists (const std::string& name);

std::string numerate_name(std::string name, std::string ending=".png", int max_tests=200);

void savePNG(std::string filename, int windowWidth, int windowHeight, int num_chanels, unsigned char* pixels);

void saveScreenshotToFile(std::string filename, int windowWidth, int windowHeight);


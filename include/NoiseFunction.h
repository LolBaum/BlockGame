//
// Created by Baum on 01.02.2025.
//
#pragma once
#ifndef BLOCKGAME_NOISE_FUNCTIONS_H
#define BLOCKGAME_NOISE_FUNCTIONS_H

#include "glm/glm.hpp"
#include <glm/gtc/noise.hpp>
#include <iostream>


float perlinNoise(float x, float y, float z, int octaves, float base);

float EvaluateFBM(float x, float y, float z,
                  float amplitude, float frequency,
                  int octaveCount, float persistence, float lacunarity);

float EvaluateFBM(float x, float y,
                  float amplitude, float frequency,
                  int octaveCount, float persistence, float lacunarity);



#endif //BLOCKGAME_NOISE_FUNCTIONS_H

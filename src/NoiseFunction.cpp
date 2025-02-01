//
// Created by Baum on 01.02.2025.
//

#include "NoiseFunction.h"

float perlinNoise(float x, float y, float z, int octaves, float base) {
    float val = 0;
    float b = base;
    for (int i = 0; i < octaves; ++i) {
        val += glm::perlin(glm::vec3{x/b, y/b, z/b});
        b *= 2;
//        std::cout << glm::perlin(glm::vec3(x*b, y*b, z*b)) << std::endl;
    }
    val = val/((float)octaves);
//    std::cout << val << std::endl;
    return val;
}

float EvaluateFBM(float x, float y, float z, float amplitude, float frequency, int octaveCount, float persistence,
                  float lacunarity) {

    float value = 0;
    for (int i = 0; i < octaveCount; i++) {
        value += amplitude * glm::perlin(glm::vec3{x * frequency, y * frequency, z * frequency});
        amplitude *= persistence;
        frequency *= lacunarity;
    }

    return value;
}

float EvaluateFBM(float x, float y, float amplitude, float frequency, int octaveCount, float persistence, float lacunarity) {

    float value = 0;
    for (int i = 0; i < octaveCount; i++) {
        value += amplitude * glm::perlin(glm::vec2{x * frequency, y * frequency});
        amplitude *= persistence;
        frequency *= lacunarity;
    }

    return value;
}

#pragma once

#include <vk_types.h>

#include <glm/glm.hpp>
#include <glm/vec3.hpp>

#include <iostream>
#include <random>
#include <algorithm>
#include <vector>


class noiseGenerator {
public:
	int numOctaves = 8;
	float amplitude_start = 1.0f,
		  frequency_start = 0.005f;

	std::vector<int> permutations;

	void init_permuation();
	float perlin_noise3D(float x, float y, float z);
	float perlin_noise2D(float x, float y);
	glm::vec2 getConstantVector(int v);

	float fade(float t);
	float lerp(float t, float a, float b);
	float grad(int hash, float x, float y, float z);
	float dot_product2D(glm::vec2 vec1, glm::vec2 vec2);

	float fractalBrownianMotion2D(float x, float y);
	float fractalBrownianMotion3D(float x, float y, float z);

private:
};
#include "noise_generator.h"

void noiseGenerator::init_permuation()
{
	auto rd = std::random_device{};
	auto rng = std::default_random_engine{ rd() };

	for (int i = 0; i < 256; i++) {
		permutations.push_back(i);
	}

	std::shuffle(permutations.begin(), permutations.end(), rng);

	auto old_count = permutations.size();
	permutations.reserve(2 * old_count);
	std::copy_n(permutations.begin(), old_count, std::back_inserter(permutations));
}

float noiseGenerator::perlin_noise3D(float x, float y, float z)
{
    int X = (int)std::floor(x) & 255,                  // FIND UNIT CUBE THAT
        Y = (int)std::floor(y) & 255,                  // CONTAINS POINT.
        Z = (int)std::floor(z) & 255;

    x -= std::floor(x);                                // FIND RELATIVE X,Y,Z
    y -= std::floor(y);                                // OF POINT IN CUBE.
    z -= std::floor(z);

    float u = fade(x),                                // COMPUTE FADE CURVES
          v = fade(y),                                // FOR EACH OF X,Y,Z.
          w = fade(z);

    int A = permutations[X] + Y, AA = permutations[A] + Z, AB = permutations[A + 1] + Z,      // HASH COORDINATES OF
        B = permutations[X + 1] + Y, BA = permutations[B] + Z, BB = permutations[B + 1] + Z;      // THE 8 CUBE CORNERS,

    return lerp(w, lerp(v, lerp(u, grad(permutations[AA], x, y, z),  // AND ADD
        grad(permutations[BA], x - 1, y, z)), // BLENDED
        lerp(u, grad(permutations[AB], x, y - 1, z),  // RESULTS
            grad(permutations[BB], x - 1, y - 1, z))),// FROM  8
        lerp(v, lerp(u, grad(permutations[AA + 1], x, y, z - 1),  // CORNERS
            grad(permutations[BA + 1], x - 1, y, z - 1)), // OF CUBE
            lerp(u, grad(permutations[AB + 1], x, y - 1, z - 1),
                grad(permutations[BB + 1], x - 1, y - 1, z - 1))));
}

float noiseGenerator::perlin_noise2D(float x, float y)
{
	int X = (int)std::floor(x) & 255;
	int Y = (int)std::floor(y) & 255;

	int xf = x - std::floor(x);
	int yf = y - std::floor(y);

	glm::vec2 topRight = { xf - 1, yf - 1 };
	glm::vec2 topLeft = { xf, yf - 1 };
	glm::vec2 botRight = { xf - 1, yf };
	glm::vec2 botLeft = { xf, yf };

	// Select a value from the permutation array for each of the 4 corners
	int valueTopRight = permutations[permutations[X + 1] + Y + 1];
	int valueTopLeft = permutations[permutations[X] + Y + 1];
	int valueBotRight = permutations[permutations[X + 1] + Y];
	int valueBotLeft = permutations[permutations[X] + Y];

	float dotTopRight = dot_product2D(topRight, getConstantVector(valueTopRight));
	float dotTopLeft = dot_product2D(topLeft, getConstantVector(valueTopLeft));
	float dotBotRight = dot_product2D(botRight, getConstantVector(valueBotRight));
	float dotBotLeft = dot_product2D(botLeft, getConstantVector(valueBotLeft));

	float u = fade(xf);
	float v = fade(yf);

	return lerp(u, lerp(v, dotBotLeft, dotTopLeft), lerp(v, dotBotRight, dotTopRight));
}

glm::vec2 noiseGenerator::getConstantVector(int v)
{
	int h = v & 3;
	if (h == 0)
		return { 1.0, 1.0 };
	else if (h == 1)
		return { -1.0, 1.0 };
	else if (h == 2)
		return { -1.0, -1.0 };
	else
		return { 1.0, -1.0 };
}

float noiseGenerator::fade(float t)
{
	return t * t * t * (t * (t * 6 - 15) + 10);
}

float noiseGenerator::lerp(float t, float a, float b)
{
	return a + t * (b - a);
}

float noiseGenerator::grad(int hash, float x, float y, float z)
{
	int h = hash & 15;														// CONVERT LO 4 BITS OF HASH CODE
	double u = h < 8 ? x : y, v = h < 4 ? y : h == 12 || h == 14 ? x : z;   // INTO 12 GRADIENT DIRECTIONS.
	return ((h & 1) == 0 ? u : -u) + ((h & 2) == 0 ? v : -v);
}

float noiseGenerator::dot_product2D(glm::vec2 vec1, glm::vec2 vec2)
{
	return vec1.x * vec2.x + vec1.y * vec2.y;
}

float noiseGenerator::fractalBrownianMotion2D(float x, float y)
{
	float result = 0.0f,
		  amplitude = amplitude_start,
		  frequency = frequency_start;

	for (int i = 0; i < numOctaves; i++) {
		float n = amplitude * perlin_noise2D(x * frequency, y * frequency);

		result += n;
		amplitude *= 0.5f;
		amplitude *= 2.0f;
	}

	return result;
}

float noiseGenerator::fractalBrownianMotion3D(float x, float y, float z)
{
	float result = 0.0f,
		  amplitude = amplitude_start,
		  frequency = frequency_start;

	for (int i = 0; i < numOctaves; i++) {
		float n = amplitude * perlin_noise3D(x * frequency, y * frequency, z*frequency);

		result += n;
		amplitude *= 0.5f;
		amplitude *= 2.0f;
	}

	return result;
}

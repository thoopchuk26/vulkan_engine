#include "noise_generator.h"

void noiseGenerator::init_permuation()
{
	auto rd = std::random_device{};
	auto rng = std::default_random_engine{ rd() };

	for (int i = 0; i < 256; i++) {
		permuations.push_back(i);
	}
	std::shuffle(permuations.begin(), permutations.end(), rng);
	permutations.resize(2 * permuations.size());
	std::copy_n(permutations.begin(), 256, permutations.begin() + 256);
}

void noiseGenerator::perlin_noise3D(float x, float y, float z)
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

    int A = permuations[X] + Y, AA = permuations[A] + Z, AB = permuations[A + 1] + Z,      // HASH COORDINATES OF
        B = permuations[X + 1] + Y, BA = permuations[B] + Z, BB = permuations[B + 1] + Z;      // THE 8 CUBE CORNERS,

    return lerp(w, lerp(v, lerp(u, grad(permuations[AA], x, y, z),  // AND ADD
        grad(permuations[BA], x - 1, y, z)), // BLENDED
        lerp(u, grad(permuations[AB], x, y - 1, z),  // RESULTS
            grad(permuations[BB], x - 1, y - 1, z))),// FROM  8
        lerp(v, lerp(u, grad(permuations[AA + 1], x, y, z - 1),  // CORNERS
            grad(p[BA + 1], x - 1, y, z - 1)), // OF CUBE
            lerp(u, grad(permuations[AB + 1], x, y - 1, z - 1),
                grad(permuations[BB + 1], x - 1, y - 1, z - 1))));
}

void noiseGenerator::perlin_noise2D(float x, float y, float z)
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
	int valueTopRight = permuations[permuations[X + 1] + Y + 1];
	int valueTopLeft = permuations[permuations[X] + Y + 1];
	int valueBotRight = permuations[permuations[X + 1] + Y];
	int valueBotLeft = permuations[permuations[X] + Y];

	glm::vec2 dotTopRight = dot_product2D(topRight, getConstantVector(valueTopRight));
	glm::vec2 dotTopLeft = dot_product2D(topLeft, getConstantVector(valueTopLeft));
	glm::vec2 dotBotRight = dot_product2D(botRight, getConstantVector(valueBotRight));
	glm::vec2 dotBotLeft = dot_product2D(botLeft, getConstantVector(valueBotLeft));

	float u = fade(xf);
	float v = fade(yf);

	return lerp(u, lerp(v, dotBotLeft, dotTopLeft), lerp(v, dotBotRight, dotTopRight));
}

glm::vec2 noiseGenerator::getConstantVector(int v)
{
	const h = v & 3;
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

glm::vec2 noiseGenerator::dot_product2D(glm::vec2 vec1, glm::vec2 vec2)
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

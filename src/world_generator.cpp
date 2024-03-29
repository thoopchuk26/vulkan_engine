#include "world_generator.h"

void worldGenerator::init_positions()
{
	for (int z = 0; z < instance_count; z++) {
		for (int x = 0; x < instance_count; x++) {
			positions.push_back({ x, 1.0f, z });
		}
	}
}

void worldGenerator::generate_world()
{
	noiseGenerator gen;
	gen.init_permuation();

	for (glm::vec3 pos : positions) {
		pos.y = std::floor(gen.fractalBrownianMotion2D(pos.x, pos.z));
	}
}

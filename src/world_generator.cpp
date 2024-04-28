#include "world_generator.h"

void worldGenerator::init_positions()
{
	InstanceData temp;
	for (int z = 0; z < instance_count; z++) {
		for (int x = 0; x < instance_count; x++) {
			temp.pos = { x, 1.0f, z };
			positions.push_back(temp);
		}
	}
}

void worldGenerator::generate_world()
{
	init_positions();
	noiseGenerator gen;
	gen.init_permuation();

	for (InstanceData block : positions) {
		block.pos.y = std::floor(gen.fractalBrownianMotion2D(block.pos.x, block.pos.z));
	}
}

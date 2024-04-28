#pragma once

#include <glm/glm.hpp>
#include <noise_generator.h>
#include "vk_types.h"

#include <vector>

class worldGenerator {
public:
	int instance_count = 512;

	std::vector<InstanceData> positions;

	void init_positions();
	void generate_world();

private:
};